/*
 * @Author: jyx
 * @Date: 2025-04-16 11:14:53
 * @LastEditors: jyx
 * @Description:
 */
#include "logger.h"
#include <iostream>
#include <time.h>
#include <chrono>

Logger& Logger::GetInstance()
{
    static Logger logger;
    return logger;
}

Logger::Logger()
{
    // 启动专门的写日志线程
    std::thread writeLogTask([&](){
        for(;;)
        {
            // 获取当时的日期，取日志信息，写入相应的日志文件当中
            std::chrono::time_point now = std::chrono::system_clock::now();
            time_t nowtime = std::chrono::system_clock::to_time_t(now);
            auto tm = localtime(&nowtime);

            char file_name[128];
            sprintf(file_name, "%d-%d-%d-log.txt", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);

            FILE *pf = fopen(file_name, "a+");
            if(pf == nullptr)
            {
                std::cout << "logger file: " << file_name << "open error!" << std::endl;
                exit(EXIT_FAILURE);
            }
            std::string msg = m_lckQue.Pop();
            
            char time_buf[128] = {0};
            sprintf(time_buf, "%d:%d:%d => [%s]", tm->tm_hour, tm->tm_min, tm->tm_sec, m_loglevel ==INFO ? "info" : "error");
            msg.insert(0, time_buf);
            msg.append("\n");
            fputs(msg.c_str(), pf);
            fclose(pf);
        }
    });
    // 设置分离线程
    writeLogTask.detach();
}

// 设置日志级别
void Logger::SetLogLevel(LogLevel level)
{
    m_loglevel = level;
}

void Logger::Log(std::string msg)
{
    m_lckQue.Push(msg);
}