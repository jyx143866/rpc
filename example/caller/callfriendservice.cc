/*
 * @Author: jyx
 * @Date: 2025-04-15 14:36:53
 * @LastEditors: jyx
 * @Description:
 */
#include <iostream>
#include "mprpcapplication.h"
#include "friend.pb.h"

int main(int argc, char **argv)
{
    // 整个程序启动以后，想使用mprpc框架来享受rpc服务调用，一定要先调用框架的初始化函数（只初始化一次）
    MprpcApplication::Init(argc, argv);
    fixbug::FriendServiceRpc_Stub stub(new MprpcChannel());
    fixbug::GetFriendRequest request;
    request.set_userid(1222);
    fixbug::GetFriendResponse response;

    MprpcController controller;

    stub.GetFriendList(&controller, &request, &response, nullptr);

    if (controller.Failed())
    {
        std::cout << controller.ErrorText() << std::endl;
    }
    else
    {
        if (0 == response.result().errcode())
        {
            int size = response.friends_size();
            for (int i = 0; i < size; ++i)
            {
                std::cout << "index:" << (i + 1) << " name:" << response.friends(i) << std::endl;
            }
        }
        else
        {
            std::cout << "rpc GetFriendList response error : " << response.result().errmsg() << std::endl;
        }
    }

    return 0;
}