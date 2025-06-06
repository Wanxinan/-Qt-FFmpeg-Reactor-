#include "../include/TcpConnection.hpp"
#include "../include/TcpServer.hpp"
#include "../include/Threadpool.hpp"
#include "../include/Configuration.hpp"
#include "../include/PasswordValidator.hpp"
#include "../include/PasswordQtValidator.hpp"
#include "../include/HttpClient.hpp"
#include "../include/json.hpp"
extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
}
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
using namespace std;
using json = nlohmann::json;

enum TaskType {
    TASK_LOGIN_SECTION1 = 1,
    TASK_LOGIN_SECTION1_RESP_OK,
    TASK_LOGIN_SECTION1_RESP_ERROR,
    TASK_LOGIN_SECTION2,
    TASK_LOGIN_SECTION2_RESP_OK, 	
    TASK_LOGIN_SECTION2_RESP_ERROR,
    TASK_REGISTER_SECTION1,
    TASK_REGISTER_SECTION1_RESP_OK,
    TASK_REGISTER_SECTION1_RESP_ERROR,
    TASK_REGISTER_SECTION2,
    TASK_REGISTER_SECTION2_RESP_OK,
    TASK_REGISTER_SECTION2_RESP_ERROR,
};

enum CameraControl{
    TASK_GET_VIDEO_STREAM = 100,
    TASK_GET_VIDEO_STREAM_OK,
    TASK_GET_VIDEO_STREAM_ERROR,
    TASK_CAMERA_MOVE,
    TASK_CAMERA_MOVE_OK,
    TASK_CAMERA_MOVE_ERROR,
};

class TaskLoginSection1
{
public:
    TaskLoginSection1(const string & msg, wd::TcpConnectionPtr conn)
        : _msg(msg)
          , _conn(conn)
    {
        validatorQtPtr = make_shared<PasswordQtValidator>();
    }

    //process函数在线程池中的某一个子线程来执行的
    void process(const shared_ptr<const unordered_map<string, string>> & result)
    {
        string client = result->at("client");
        string response;
        if(client == "ubuntu"){
            string username = result->at("username");
            if(validator.hasUser(username)){
                response = to_string(TASK_LOGIN_SECTION1_RESP_OK);
            }else{
                response = to_string(TASK_LOGIN_SECTION1_RESP_ERROR);
            }
            _conn->sendInLoop(response);
        }
        else{
            string username = result->at("username");
            if(validatorQtPtr->hasUser(username)){
                string salt = validatorQtPtr->getSaltByUsername(username);
                response = "type=" + to_string(TASK_LOGIN_SECTION1_RESP_OK) + 
                    ",salt=" + salt;
            }else{
                response = "type=" + to_string(TASK_LOGIN_SECTION1_RESP_ERROR);
            }
            _conn->sendInLoop(response);
        }
    }

private:
    string _msg;
    wd::TcpConnectionPtr _conn;
    PasswordValidator validator;
    shared_ptr<PasswordQtValidator> validatorQtPtr;
};

class TaskLoginSection2
{
public:
    TaskLoginSection2(const string & msg, wd::TcpConnectionPtr conn)
        : _msg(msg)
          , _conn(conn)
    {
        validatorQtPtr = make_shared<PasswordQtValidator>();
    }
    //process函数在线程池中的某一个子线程来执行的
    void process(const shared_ptr<const unordered_map<string, string>> & result)
    {
        string client = result->at("client");
        string response;
        if(client == "ubuntu"){
            string username = result->at("username");
            string password = result->at("password");
            if(validator.validate_password(username,password)){
                response = to_string(TASK_LOGIN_SECTION2_RESP_OK);
            }else{
                response = to_string(TASK_LOGIN_SECTION2_RESP_ERROR);
            }
            _conn->sendInLoop(response);
        }else{
            string username = result->at("username");
            string crypt = result->at("crypt");
            if(validatorQtPtr->validate_password(username,crypt)){
                response = "type=" + to_string(TASK_LOGIN_SECTION2_RESP_OK);
            }else{
                response = "type=" + to_string(TASK_LOGIN_SECTION2_RESP_ERROR);
            }
            _conn->sendInLoop(response);
        }
    }

private:
    string _msg;
    wd::TcpConnectionPtr _conn;
    PasswordValidator validator;
    shared_ptr<PasswordQtValidator> validatorQtPtr;
};

class TaskRegisterSection1
{
public:
    TaskRegisterSection1(const string & msg, wd::TcpConnectionPtr conn)
        : _msg(msg)
          , _conn(conn)
    {
        validatorQtPtr = make_shared<PasswordQtValidator>();
    }

    //process函数在线程池中的某一个子线程来执行的
    void process(const shared_ptr<const unordered_map<string, string>> & result)
    {
        string client = result->at("client");
        string response;
        if(client == "ubuntu"){
        }
        else{
            string username = result->at("username");
            if(validatorQtPtr->hasUser(username)){
                //注册的第一步，如果用户存在，则无法注册
                response = "type=" + to_string(TASK_REGISTER_SECTION1_RESP_ERROR);
            }else{
                //用户不存在，生成随机盐值返回客户端
                string salt = validatorQtPtr->GenRandomString(30);
                response = "type=" + to_string(TASK_REGISTER_SECTION1_RESP_OK) + 
                    ",salt=" + salt;
            }
            _conn->sendInLoop(response);
        }
    }

private:
    string _msg;
    wd::TcpConnectionPtr _conn;
    PasswordValidator validator;
    shared_ptr<PasswordQtValidator> validatorQtPtr;
};

class TaskRegisterSection2
{
public:
    TaskRegisterSection2(const string & msg, wd::TcpConnectionPtr conn)
        : _msg(msg)
          , _conn(conn)
    {
        validatorQtPtr = make_shared<PasswordQtValidator>();
    }

    //process函数在线程池中的某一个子线程来执行的
    void process(const shared_ptr<const unordered_map<string, string>> & result)
    {
        string client = result->at("client");
        string response;
        if(client == "ubuntu"){
        }
        else{
            string username = result->at("username");
            string salt = result->at("salt");
            string ctypt = result->at("ctypt");
            if(validatorQtPtr->insertUser(username,salt,ctypt)){
                response = "type=" + to_string(TASK_REGISTER_SECTION2_RESP_OK);
            }else{
                response = "type=" + to_string(TASK_REGISTER_SECTION2_RESP_ERROR);
            }
            _conn->sendInLoop(response);
        }
    }

private:
    string _msg;
    wd::TcpConnectionPtr _conn;
    PasswordValidator validator;
    shared_ptr<PasswordQtValidator> validatorQtPtr;
};


class TaskGetVideoStream
{
public:
    TaskGetVideoStream(const string & msg, wd::TcpConnectionPtr conn)
        : _msg(msg)
          , _conn(conn)
    {
        validatorQtPtr = make_shared<PasswordQtValidator>();
    }

string base64Encode(const uint8_t* data, size_t length) {
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); // 禁用换行符
    BIO_write(bio, data, length);
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);
    string result(bufferPtr->data, bufferPtr->length);
    BIO_free_all(bio);
    return result;
}

    //process函数在线程池中的某一个子线程来执行的
    void process(const shared_ptr<const unordered_map<string, string>> & result)
    {
        string client = result->at("client");
        string response;
        if(client == "ubuntu"){
        }
        else{
            string username = result->at("username");
            string url = result->at("url");
            cout << "username = " << username << endl;
            cout << "url = " << url << endl;
            if(validatorQtPtr->hasUser(username)){
                AVFormatContext *pFormatCtx = nullptr; //音视频封装格式上下文结构体
                AVCodecContext  *pCodecCtx = nullptr;  //音视频编码器上下文结构体
                AVCodec *pCodec = nullptr; //音视频编码器结构体
                AVFrame *pFrame = nullptr; //存储一帧解码后像素数据
                AVFrame *pFrameRGB = nullptr;
                AVPacket *pPacket = nullptr; //存储一帧压缩编码数据
                uint8_t *pOutBuffer = nullptr;
                static struct SwsContext *pImgConvertCtx = nullptr;
                avformat_network_init();   //初始化FFmpeg网络模块
                pFormatCtx = avformat_alloc_context();
                AVDictionary *avdic = nullptr;
                char option_key[] = "rtsp_transport";
                char option_value[] = "udp";
                av_dict_set(&avdic, option_key, option_value, 0);
                char option_key2[] = "max_delay";
                char option_value2[] = "100";
                av_dict_set(&avdic, option_key2, option_value2, 0);
                av_dict_set(&avdic, "analyzeduration", "1000000", 0);  // 增加分析时间
                av_dict_set(&avdic, "probesize", "10000000", 0);       // 增加探测大小
                                                                       //if (avformat_open_input(&pFormatCtx, url.c_str(), nullptr, &avdic) != 0) {
                if (avformat_open_input(&pFormatCtx, url.c_str(), nullptr, nullptr) != 0) {
                    printf("can't open the file. \n");
                    return;
                }
                if(!pFormatCtx){
                    cout << "pFormatCtx == nullptr" << endl;
                    return;
                }
                if (avformat_find_stream_info(pFormatCtx, nullptr) < 0) {
                    printf("Could't find stream infomation.\n");
                    return;
                }
                cout << "n_streams:" << pFormatCtx->nb_streams << endl;
                // 输出流信息
                for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++) {
                    AVCodecParameters *codec_par = pFormatCtx->streams[i]->codecpar;
                    printf("Stream %d: Codec ID: %d, Width: %d, Height: %d\n",
                           i, codec_par->codec_id, codec_par->width, codec_par->height);
                }
                //查找视频中包含的流信息，音频流先不处理
                int videoStreamIdx = -1;
                videoStreamIdx = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
                if(videoStreamIdx < 0) {
                    return ;
                }
                //查找解码器
                pCodec = avcodec_find_decoder(pFormatCtx->streams[videoStreamIdx]->codecpar->codec_id);
                if (pCodec == nullptr) {
                    return;
                }
                //开辟解码器空间
                pCodecCtx = avcodec_alloc_context3(pCodec);
                //拷贝解码器参数
                avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[videoStreamIdx]->codecpar);
                //初始化解码器参数
                cout << "" << endl;
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
                int i = 1;
                while (i--) {
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
                            //给客户端传递数据
                            int numBytes = avpicture_get_size(AV_PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height);
                            string buffer = base64Encode(pOutBuffer, numBytes);
                            cout << "buffer = " << buffer << ",width = " << pCodecCtx->width 
                                << ",height = " << pCodecCtx->height << endl;
                            cout << "一帧图片大小为" << buffer.size() << endl;
                            response = "type=" + to_string(TASK_GET_VIDEO_STREAM_OK) +
                                ",width=" + to_string(pCodecCtx->width) + 
                                ",height=" + to_string(pCodecCtx->height) + 
                                ",length=" + to_string(buffer.size()) +
                                 ",data=" + buffer;
                            cout << "responsesize = " <<  response.size() << endl;
                            _conn->sendInLoop(response);
                        }
                    }
                    av_free_packet(pPacket);
                }
                av_free(pOutBuffer);
                av_free(pFrameRGB);
                avcodec_close(pCodecCtx);
                avformat_close_input(&pFormatCtx);
            }else{
                response = "type=" + to_string(TASK_GET_VIDEO_STREAM_ERROR);
                _conn->sendInLoop(response);
            }
            }
        }

    private:
        string _msg;
        wd::TcpConnectionPtr _conn;
        PasswordValidator validator;
        shared_ptr<PasswordQtValidator> validatorQtPtr;
    };

    class TaskCameraMove
    {
    public:
        TaskCameraMove(const string & msg, wd::TcpConnectionPtr conn)
            : _msg(msg)
              , _conn(conn)
        {
            validatorQtPtr = make_shared<PasswordQtValidator>();
            httpclient = make_shared<HttpClient>();
        }

        //process函数在线程池中的某一个子线程来执行的
        void process(const shared_ptr<const unordered_map<string, string>> & result)
        {
            string client = result->at("client");
            string response;
            if(client == "ubuntu"){
            }
            else{
                string username = result->at("username");
                string url = result->at("url");
                string http_response = httpclient->get(url);
                cout << "json : " << http_response << endl;
                auto http_result = httpclient->parse_json_to_string_map(http_response);
                string msg = http_result["msg"];
                string code = http_result["code"];
                cout << "code = " << code << endl;
                if(code == "0"){
                    response = "type=" + to_string(TASK_CAMERA_MOVE_OK);
                }else{
                    response = "type=" + to_string(TASK_CAMERA_MOVE_ERROR) + msg;
                }
                cout << response << endl;
                _conn->sendInLoop(response);
            }
        }

    private:
        string _msg;
        wd::TcpConnectionPtr _conn;
        PasswordValidator validator;
        shared_ptr<PasswordQtValidator> validatorQtPtr;
        shared_ptr<HttpClient> httpclient;
    };
    class EchoServer
    {
    public:
        EchoServer(int num, int quesize, unsigned short port, const string & ip = "0.0.0.0")
            : _threadpool(num, quesize)
            , _server(port, ip)
            {
                //命名空间的使用是限定在构造函数中了,减小其使用范围
                using namespace std::placeholders;
                _server.setAllCallbacks(
                                        std::bind(&EchoServer::onConnection, this, _1),
                                        std::bind(&EchoServer::onMessage, this, _1),
                                        std::bind(&EchoServer::onClose, this, _1));
            }

        void start()
        {
            _threadpool.start();
            _server.start();
        }


        void onConnection(wd::TcpConnectionPtr conn)
        {
            cout << conn->toString() << " has connected" << endl;
        }

        unordered_map<string, string> splitKeyValuePairs(const string &input) {
            unordered_map<string, string>  result;
            size_t start = 0;
            size_t end = input.find(',');

            while (end != string::npos) {
                string pair = input.substr(start, end - start);
                size_t equalSignPos = pair.find('=');
                if (equalSignPos != string::npos) {
                    string key = pair.substr(0, equalSignPos);
                    string value = pair.substr(equalSignPos + 1);
                    result[key] = value; // 直接插入到 unordered_map
                }
                start = end + 1;
                end = input.find(',', start);
            }

            // 处理最后一个键值对
            string pair = input.substr(start);
            size_t equalSignPos = pair.find('=');
            if (equalSignPos != string::npos) {
                string key = pair.substr(0, equalSignPos);
                string value = pair.substr(equalSignPos + 1);
                result[key] = value; // 直接插入到 unordered_map
            }

            return result;
        }

        //这是消息到达时候的处理方式
        void onMessage(wd::TcpConnectionPtr conn)
        {
            //获取消息
            string msg = conn->receive();
            cout << "msg:" << msg << endl;
            unordered_map<string, string> result = splitKeyValuePairs(msg);
            string type = result["type"];
            int taskType = atoi(type.c_str());
            cout << "taskType = " << taskType << endl;
            auto resultPtr = make_shared<const unordered_map<string, string>>(result);
            //IO线程 -> 计算线程
            switch(taskType){
            case TASK_LOGIN_SECTION1:{
                                         TaskLoginSection1 tasklogin1(msg, conn);
                                         _threadpool.addTask(std::bind(&TaskLoginSection1::process, tasklogin1,resultPtr));
                                         break;
                                     }
            case TASK_LOGIN_SECTION2:{
                                         TaskLoginSection2 tasklogin2(msg, conn);
                                         _threadpool.addTask(std::bind(&TaskLoginSection2::process, tasklogin2,resultPtr));
                                         break;
                                     }
            case TASK_REGISTER_SECTION1:{
                                            TaskRegisterSection1 taskregister1(msg, conn);
                                            _threadpool.addTask(std::bind(&TaskRegisterSection1::process, taskregister1,resultPtr));
                                            break;
                                        }
            case TASK_REGISTER_SECTION2:{
                                            TaskRegisterSection2 taskregister2(msg, conn);
                                            _threadpool.addTask(std::bind(&TaskRegisterSection2::process, taskregister2,resultPtr));
                                            break;
                                        }
            case TASK_GET_VIDEO_STREAM:{
                                           TaskGetVideoStream taskgetvideostream(msg, conn);
                                           _threadpool.addTask(std::bind(&TaskGetVideoStream::process, taskgetvideostream,resultPtr));
                                           break;
                                       }
            case TASK_CAMERA_MOVE:{
                                      TaskCameraMove taskcameramove(msg, conn);
                                      _threadpool.addTask(std::bind(&TaskCameraMove::process, taskcameramove,resultPtr));
                                      break;
                                  }
            }
        }

        void onClose(wd::TcpConnectionPtr conn)
        {
            cout << conn->toString() << " has closed." << endl;
        }


    private:
        wd::Threadpool _threadpool;
        wd::TcpServer _server;
    };


    int main(void)
    {
        Configuration & config = Configuration::getInstance();

        int thread_num = stoi(config.getConfigValue("thread_num"));
        int task_num = stoi(config.getConfigValue("task_num"));
        short port = static_cast<short>(stoi(config.getConfigValue("port")));
        string ip = config.getConfigValue("ip");

        //config.printConfig();
        EchoServer server(thread_num, task_num, port,ip);
        server.start();

        return 0;
    }
