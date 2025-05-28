#ifndef __WD_MYLOGGER_H__
#define __WD_MYLOGGER_H__
#include <iostream>

#include <log4cpp/Category.hh>

using std::cout;
using std::endl;


class Mylogger
{
    class AutoRelease
    {
    public:
        AutoRelease() {}
        ~AutoRelease() {
            if(_pInstance) {
                delete _pInstance;
            }
        }
    };
public:
    static Mylogger * getInstance()
    {
        if(nullptr == _pInstance) {
            _pInstance = new Mylogger();
        }
        return _pInstance;
    }

    static void destroy()
    {
        if(_pInstance) {
            delete _pInstance;
            _pInstance = nullptr;
        }
    }

    void error(const char * msg);

    template <class... Args>
    void error(const char * msg, Args... args)
    {
        _cat.error(msg, args...);
    }


    void warn(const char * msg);

    template <class... Args>
    void warn(const char * msg, Args... args)
    {
        _cat.warn(msg, args...);
    }

    void debug(const char * msg);

    template <class... Args>
    void debug(const char * msg, Args... args)
    {
        _cat.debug(msg, args...);
    }

    void info(const char * msg);

    template <class... Args>
    void info(const char * msg, Args... args)
    {
        _cat.info(msg, args...);
    }


private:
    Mylogger();
    ~Mylogger();

private:
    static Mylogger * _pInstance;
    log4cpp::Category & _cat;
    //log4cpp::Appender * _pOstreamAppender;
    static AutoRelease _ar;
};

#define addprefix(msg)  string("[")\
    .append(__FILE__).append(":")\
    .append(__func__).append(":")\
    .append(std::to_string(__LINE__)).append("] ")\
    .append(msg).c_str()

#define LogInfo(msg, ...) Mylogger::getInstance()->info(addprefix(msg), ##__VA_ARGS__)
#define LogError(msg, ...) Mylogger::getInstance()->error(addprefix(msg), ##__VA_ARGS__)
#define LogWarn(msg, ...) Mylogger::getInstance()->warn(addprefix(msg), ##__VA_ARGS__)
#define LogDebug(msg, ...) Mylogger::getInstance()->debug(addprefix(msg), ##__VA_ARGS__)

#endif
