/***
 * @Author: jyx
 * @Date: 2025-04-10 08:21:50
 * @LastEditors: jyx
 * @Description:
 */
#pragma once
#include "mprpcconfig.h"

// rpc框架的基础类
class MprpcApplication{
public:
    static void Init(int argc, char **argv);
    static MprpcApplication& GetInstance();
    static MprpcConfig& GetConfig();
private:
    static MprpcConfig m_config;

    MprpcApplication();
    MprpcApplication(const MprpcApplication&) = delete;
    MprpcApplication(const MprpcApplication&&) = delete;
};