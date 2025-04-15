/*
 * @Author: jyx
 * @Date: 2025-04-15 05:29:25
 * @LastEditors: jyx
 * @Description:
 */
#include <iostream>
#include "mprpcapplication.h"
#include "user.pb.h"
#include "mprpcchannel.h"


int main(int argc, char **argv)
{
    // 整个程序启动以后，想使用mprpc框架来享受rpc服务调用，一定要先调用框架的初始化函数（只初始化一次）
    MprpcApplication::Init(argc, argv);
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());
    fixbug::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123456");
    fixbug::LoginResponse response;

    stub.Login(nullptr, &request, &response, nullptr);

    if(0 == response.result().errcode()){
        std::cout << "rpc login response success : " << response.success() << std::endl;
    }
    else{
        std::cout << "rpc login response error : " << response.result().errmsg() << std::endl;
    }


    fixbug::RegisterRequest req;
    req.set_id(2000);
    req.set_name("jyx");
    req.set_pwd("123456");
    fixbug::RegisterResponse rsp;

    stub.Register(nullptr, &req, &rsp, nullptr);

    if (0 == response.result().errcode())
    {
        std::cout << "rpc Register response success : " << rsp.success() << std::endl;
    }
    else
    {
        std::cout << "rpc Register response error : " << rsp.result().errmsg() << std::endl;
    }
    

    return 0;
}