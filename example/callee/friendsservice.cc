/*
 * @Author: jyx
 * @Date: 2025-04-15 10:02:40
 * @LastEditors: jyx
 * @Description:
 */
#include <iostream>
#include <string>
#include "friend.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"
#include "logger.h"

class FriendService : public fixbug::FriendServiceRpc
{
public:
    std::vector<std::string> GetFriendList(uint32_t userid)
    {
        std::cout << "do GetFriendList service" << std::endl;
        std::vector<std::string> vec;
        vec.push_back("lyh");
        vec.push_back("jyx");
        vec.push_back("gjw");
        return vec;
    }

    void GetFriendList(google::protobuf::RpcController *controller, const ::fixbug::GetFriendRequest *request, ::fixbug::GetFriendResponse *response, ::google::protobuf::Closure *done)
    {
        uint32_t userid = request->userid();
        std::vector<std::string> friendList = GetFriendList(userid);
        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg(" ");
        for(std::string &name : friendList)
        {
            std::string *p = response->add_friends();
            *p = name;
        }
        done->Run();
    }
};

int main(int argc, char **argv)
{
    LOG_INFO("first log message!");
    LOG_ERR("%s:%s:%d", __FILE__, __FUNCTION__, __LINE__);
    // 调用框架初始化操作
    MprpcApplication::Init(argc, argv);
    // provider是一个rpc网络服务对象，把UserService对象发布到rpc节点上
    RpcProvider provider;
    provider.NotifyService(new FriendService());

    // 启动一个rpc服务发布节点，run以后，进程进入阻塞状态，等待远程rpc调用请求
    provider.Run();
    return 0;
}