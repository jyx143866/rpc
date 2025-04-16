/*
 * @Author: jyx
 * @Date: 2025-04-15 05:53:04
 * @LastEditors: jyx
 * @Description:
 */
#include "mprpcchannel.h"
#include "rpcheader.pb.h"
#include "mprpcapplication.h"
#include "mprpccontroller.h"
#include <string>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <error.h>
#include <format>

void MprpcChannel::CallMethod(const google::protobuf::MethodDescriptor *method, google::protobuf::RpcController *controller, const google::protobuf::Message *request, google::protobuf::Message *response, google::protobuf::Closure *done){
    const google::protobuf::ServiceDescriptor* sd = method->service();
    std::string service_name = sd->name();
    std::string method_name = method->name();

    // 获取参数的序列化字符串长度 args_size
    std::string args_str;
    uint32_t args_size = 0;
    if(request->SerializeToString(&args_str)){
        args_size = args_str.size();
    }
    else
    {
        controller->SetFailed("serialize request error");
        return;
    }

    // 定义rpc的请求header
    mprpc::RpcHeader rpcheader;
    rpcheader.set_service_name(service_name);
    rpcheader.set_method_name(method_name);
    rpcheader.set_args_size(args_size);

    uint32_t header_size = 0;
    std::string rpc_header_str;
    if(rpcheader.SerializeToString(&rpc_header_str)){
        header_size = rpc_header_str.size();
    }
    else
    {
        controller->SetFailed("serialize rpc header error!");
    }

    std::string send_rpc_str;
    send_rpc_str.insert(0, std::string((char*)&header_size, 4));
    send_rpc_str += rpc_header_str;
    send_rpc_str += args_str;

    std::cout << "header_size:" << header_size << std::endl;
    std::cout << "rpc_header_str:" << rpc_header_str << std::endl;
    std::cout << "service_name:" << service_name << std::endl;
    std::cout << "method_name:" << method_name << std::endl;
    std::cout << "args_str:" << args_str << std::endl;

    // 使用TCP编程，完成rpc方法的远程调用
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == clientfd){
        char errtxt[512] = {0};
        sprintf(errtxt, "create clientfd error! errno:%d", errno);
        controller->SetFailed(errtxt);
        return;
    }

    // std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    // uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
    
    ZkClient zkCli;
    zkCli.start();
    std::string method_path = "/" + service_name + "/" + method_name;
    std::string host_date = zkCli.GetData(method_path.c_str());
    if(host_date == "")
    {
        controller->SetFailed(method_path + " is not exist!");
        return;
    }
    int idx = host_date.find(":");
    if(idx == -1)
    {
        controller->SetFailed(method_path + "address is invalid!");
        return;
    }
    std::string ip = host_date.substr(0, idx);
    uint16_t port = atoi(host_date.substr(idx + 1, host_date.size() - idx).c_str());

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    // 连接rpc服务节点
    if(-1 == connect(clientfd, (struct sockaddr*)&server_addr, sizeof(server_addr))){
        char errtxt[512] = {0};
        sprintf(errtxt, "connect error! errno : %d", errno);
        controller->SetFailed(errtxt);
        close(clientfd);
        return;
    }
    
    // 发送rpc请求
    if(-1 == send(clientfd, send_rpc_str.c_str(), send_rpc_str.size(), 0)){
        char errtxt[512] = {0};
        sprintf(errtxt, "send error! errno : %d", errno);
        controller->SetFailed(errtxt);
        close(clientfd);
        return;
    }

    char recv_buf[1024] = {0};
    int recv_size = 0;
    if(-1 == (recv_size = recv(clientfd, recv_buf, 1024, 0))){
        char errtxt[512] = {0};
        sprintf(errtxt, "recv error! errno : %d", errno);
        controller->SetFailed(errtxt);
        close(clientfd);
        return;
    }

    // std::string response_str(recv_buf, 0, recv_size); bug问题，存储的是二进制文件，会有\0，而使用string构造时，遇到\0就会认为字符串终止
    if(!response->ParseFromArray(recv_buf, recv_size)){
        char errtxt[1024] = {0};
        snprintf(errtxt, sizeof(errtxt), "parse error! errno : %.500s", recv_buf);
        controller->SetFailed(errtxt);
        return;
    }
    close(clientfd);
}