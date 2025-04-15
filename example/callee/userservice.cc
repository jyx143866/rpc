/*
 * @Author: jyx
 * @Date: 2025-04-10 07:43:50
 * @LastEditors: jyx
 * @Description:
 */
#include <iostream>
#include <string>
#include "user.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"

// userservice原来是一个本地方法，提供了两个进程内的本地方法，Login和GetFriendLists
class UserService : public fixbug::UserServiceRpc
{ // 使用在rpc服务提供者
public:
    bool Login(std::string name, std::string pwd)
    {
        std::cout << "Doing Local Service : Login" << std::endl;
        std::cout << "name : " << name << " pwd :" << pwd << std::endl;
        return true;
    }

    bool Register(uint32_t id, std::string name, std::string pwd)
    {
        std::cout << "Doing Local Service : Register" << std::endl;
        std::cout << "name : " << name << " pwd :" << pwd << std::endl;
        return true;
    }

    void Login(::google::protobuf::RpcController *controller,
               const ::fixbug::LoginRequest *request,
               ::fixbug::LoginResponse *response,
               ::google::protobuf::Closure *done)
    {
        // 框架给业务上报了请求参数LoginRequest，应用获取相应数据做本地业务
        std::string name = request->name();
        std::string pwd = request->pwd();

        // 做本地业务
        bool login_result = Login(name, pwd);

        // 把响应写入
        fixbug::ResultCode *code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_success(login_result);

        // 执行回调操作 执行响应对象数据的序列化和网络发送(都是由框架完成的)
        
        done->Run();
    }

    void Register(google::protobuf::RpcController *controller,
                  const ::fixbug::RegisterRequest *request,
                  ::fixbug::RegisterResponse *response,
                  ::google::protobuf::Closure *done){
        uint32_t id  = request->id();
        std::string name = request->name();
        std::string pwd = request->pwd();

        bool ret = Register(id, name, pwd);

        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("登陆成功");
        response->set_success(ret);

        done->Run();
    }
};

int main(int argc, char **argv)
{

    // 调用框架初始化操作
    MprpcApplication::Init(argc, argv);
    // provider是一个rpc网络服务对象，把UserService对象发布到rpc节点上
    RpcProvider provider;
    provider.NotifyService(new UserService());

    // 启动一个rpc服务发布节点，run以后，进程进入阻塞状态，等待远程rpc调用请求
    provider.Run();
    return 0;
}