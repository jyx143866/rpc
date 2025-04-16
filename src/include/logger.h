/***
 * @Author: jyx
 * @Date: 2025-04-16 11:14:59
 * @LastEditors: jyx
 * @Description:
 */
#pragma once
#include "lockqueue.h"
#include <string>

enum LogLevel
{
    INFO,
    ERROR,
};

// Mprpc提供的日志系统
class Logger
{
public:
    static Logger& GetInstance();
    // 设置日志级别
    void SetLogLevel(LogLevel level);
    void Log(std::string msg);
private:
    int m_loglevel; // 日志级别
    LockQueue<std::string> m_lckQue;

    Logger();
    Logger(const Logger&) = delete;
    Logger(const Logger&&) = delete;
};


#define LOG_INFO(logmsgformat, ...)\
    do \
    {\
        Logger &logger = Logger::GetInstance(); \
        logger.SetLogLevel(INFO); \
        char msg[1024] = {0}; \
        snprintf(msg, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.Log(msg); \
    } while (0);

#define LOG_ERR(logmsgformat, ...)                       \
    do                                                    \
    {                                                     \
        Logger &logger = Logger::GetInstance();           \
        logger.SetLogLevel(ERROR);                         \
        char msg[1024] = {0};                             \
        snprintf(msg, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.Log(msg);                                  \
    } while (0);
    