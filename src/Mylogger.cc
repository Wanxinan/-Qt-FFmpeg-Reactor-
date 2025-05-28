#include "Mylogger.hpp"
#include "Configuration.hpp"
//#include "Configuration.hpp"
#include <iostream>

#include <log4cpp/PatternLayout.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/Priority.hh>

using std::cout;
using std::endl;
using std::string;

using namespace log4cpp;

Mylogger * Mylogger::_pInstance = nullptr;
Mylogger::AutoRelease Mylogger::_ar;

Mylogger::Mylogger()
: _cat(log4cpp::Category::getInstance("wd"))
{
    cout << "Mylogger()" << endl;
    //1.创建日志布局对象
    auto ptn1 = new PatternLayout();
    ptn1->setConversionPattern("%d %c [%p] %m%n");
    auto ptn2 = new PatternLayout();
    ptn2->setConversionPattern("%d %c [%p] %m%n");

    //2.创建日志目的地对象
    auto pOsAppender = new OstreamAppender("console", &cout);
    pOsAppender->setLayout(ptn1);
    //从配置文件中获取日志存放路径
    string logfile = wd::Configuration::getInstance()->getConfigMap()["log_file"];
    cout << "set log file:" << logfile << endl;
    auto pFileAppender = new FileAppender("fileAppender", logfile);
    pFileAppender->setLayout(ptn2);

    //3.设置Category对象
    _cat.setPriority(Priority::DEBUG);
    _cat.addAppender(pOsAppender);
    _cat.addAppender(pFileAppender);
}

Mylogger::~Mylogger()
{
    cout << "~Mylogger()" << endl;
    //shutdown函数会回收日志布局，目的地的堆对象
    Category::shutdown();
}

void Mylogger::error(const char * msg)
{
    _cat.error(msg);
}

void Mylogger::warn(const char * msg)
{
    _cat.warn(msg);
}

void Mylogger::debug(const char * msg)
{
    _cat.debug(msg);
}

void Mylogger::info(const char * msg)
{
    _cat.info(msg);
}
