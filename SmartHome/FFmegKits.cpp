#include "FFmegKits.h"
#include <QDebug>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
}

FFmpegKits::FFmpegKits(QObject *parent)
    : QThread(parent)
{
}

void FFmpegKits::startPlay(QString url)
{
    _url = url;
    this->start();//开始运行耗时线程
}

void FFmpegKits::run()
{
    qDebug() << "FFmpegKits::run()..." << endl;
    AVFormatContext *pFormatCtx = nullptr; //音视频封装格式上下文结构体
    AVCodecContext  *pCodecCtx = nullptr;  //音视频编码器上下文结构体
    AVCodec *pCodec = nullptr; //音视频编码器结构体
    AVFrame *pFrame = nullptr; //存储一帧解码后像素数据
    AVFrame *pFrameRGB = nullptr;
    AVPacket *pPacket = nullptr; //存储一帧压缩编码数据
    uint8_t *pOutBuffer = nullptr;
    static struct SwsContext *pImgConvertCtx = nullptr;
    avformat_network_init();   //初始化FFmpeg网络模块
    //Allocate an AVFormatContext.
    pFormatCtx = avformat_alloc_context();
    //AVDictionary
    AVDictionary *avdic = nullptr;
    char option_key[] = "rtsp_transport";
    char option_value[] = "udp";
    av_dict_set(&avdic, option_key, option_value, 0);
    char option_key2[] = "max_delay";
    char option_value2[] = "100";
    av_dict_set(&avdic, option_key2, option_value2, 0);
    if (avformat_open_input(&pFormatCtx, _url.toStdString().data(), nullptr, &avdic) != 0) {
        printf("can't open the file. \n");
        return;
    }
    if (avformat_find_stream_info(pFormatCtx, nullptr) < 0) {
        printf("Could't find stream infomation.\n");
        return;
    }
    //查找视频中包含的流信息，音频流先不处理
    int videoStreamIdx = -1;
    qDebug("apFormatCtx->nb_streams:%d", pFormatCtx->nb_streams);
    videoStreamIdx = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if(videoStreamIdx < 0) {
        qDebug() << "av_find_best_stream error:" << av_get_media_type_string(AVMEDIA_TYPE_VIDEO);
        return ;
    }
    qDebug("video stream idx: %d\n", videoStreamIdx);
    //查找解码器
    qDebug("avcodec_find_decoder...\n");
    qDebug() << "AVCodecID:" << pFormatCtx->streams[videoStreamIdx]->codecpar->codec_id << endl;
    pCodec = avcodec_find_decoder(pFormatCtx->streams[videoStreamIdx]->codecpar->codec_id);
    if (pCodec == nullptr) {
        qDebug("Codec not found.\n");
        return;
    }
    //开辟解码器空间
    pCodecCtx = avcodec_alloc_context3(pCodec);
    //拷贝解码器参数
    avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[videoStreamIdx]->codecpar);
    qDebug("pCodecCtx->codec_id: %d\n", pCodecCtx->codec_id);
    //初始化解码器参数
    pCodecCtx->bit_rate = 0;   //初始化为0
    pCodecCtx->time_base.num = 1;  //下面两行：一秒钟25帧
    pCodecCtx->time_base.den = 25;
    pCodecCtx->frame_number = 1;  //每包一个视频帧
    //打开解码器
    if (avcodec_open2(pCodecCtx, pCodec, nullptr) < 0) {
        printf("Could not open codec.\n");
        return;
    }
    //将解码后的YUV数据转换成RGB32
    //创建转换上下文
    pImgConvertCtx = sws_getContext(
                         pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
                         pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB32,
                         SWS_BICUBIC, nullptr, nullptr, nullptr);
    int numBytes = avpicture_get_size(AV_PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height);
    //准备源和目标数据缓冲区
    pFrame     = av_frame_alloc();//开辟空间
    pFrameRGB  = av_frame_alloc();//开辟空间
    pOutBuffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    avpicture_fill((AVPicture *) pFrameRGB, pOutBuffer,
                   AV_PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height);
    pPacket = (AVPacket *) malloc(sizeof(AVPacket)); //分配一个packet
    int y_size = pCodecCtx->width * pCodecCtx->height;
    av_new_packet(pPacket, y_size); //分配packet的数据
    while (1) {
        if (av_read_frame(pFormatCtx, pPacket) < 0) {
            break; //这里认为视频读取完了
        }
        if (pPacket->stream_index == videoStreamIdx) {
            int got_picture;
            //执行解码
            int ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, pPacket);
            if (ret < 0) {
                printf("decode error.\n");
                return;
            }
            if (got_picture) {
                //执行转换
                sws_scale(pImgConvertCtx, (uint8_t const * const *) pFrame->data, pFrame->linesize,
                          0, pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);
                //把这个RGB数据 用QImage加载
                QImage tmpImg((uchar *)pOutBuffer,
                              pCodecCtx->width, pCodecCtx->height, QImage::Format_RGB32);
                QImage image = tmpImg.copy(); //把图像复制一份 传递给界面显示
                emit sigGetOneFrame(image);  //发送信号
            }
        }
        av_free_packet(pPacket);
    }
    av_free(pOutBuffer);
    av_free(pFrameRGB);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
}
