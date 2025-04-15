/*
 * @Author: jyx
 * @Date: 2025-04-10 08:32:40
 * @LastEditors: jyx
 * @Description:
 */
#include "mprpcapplication.h"
#include <iostream>
#include <unistd.h>
#include <string>

MprpcConfig MprpcApplication::m_config;

void showArgsHelp()
{
    std::cout << "format: command -i <configfile>" << std::endl;
}

MprpcApplication::MprpcApplication() {}

void MprpcApplication::Init(int argc, char **argv)
{
    if (argc < 2)
    {
        showArgsHelp();
        exit(EXIT_FAILURE);
    }
    int c = 0;
    std::string config_file;
    while ((c = getopt(argc, argv, "i:")) != -1)
    {
        switch (c)
        {
        case 'i':
            config_file = optarg;
            break;
        case '?':
            showArgsHelp();
            exit(EXIT_FAILURE);
        case ':':
            showArgsHelp();
            exit(EXIT_FAILURE);
        }
    }

    // 开始加载配置文件
    m_config.LoadConfigFile(config_file.c_str());

    std::cout << "rpcserverip:" << m_config.Load("rpcserverip") << std::endl;
    std::cout << "rpcserverport:" << m_config.Load("rpcserverport") << std::endl;
    std::cout << "zookeeperip:" << m_config.Load("zookeeperip") << std::endl;
    std::cout << "zookeeperport:" << m_config.Load("zookeeperport") << std::endl;
}

MprpcApplication &MprpcApplication::GetInstance()
{
    static MprpcApplication app;
    return app;
}

MprpcConfig &MprpcApplication::GetConfig(){
    return m_config;
}
