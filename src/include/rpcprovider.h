/***
 * @Author: jyx
 * @Date: 2025-04-10 08:27:08
 * @LastEditors: jyx
 * @Description:
 */
#pragma once
#include "google/protobuf/service.h"
#include "mprpcapplication.h"
#include <google/protobuf/descriptor.h>
#include <string>
#include <unordered_map>
#include <functional>
#include <memory>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>

//
class RpcProvider{
public:
    // 这是框架提供给外部使用的，可以发布rpc方法的函数接口
    void NotifyService(google::protobuf::Service *service);
    
    // 启动rpc服务节点，开始提供rpc远程网络调用服务
    void Run();
private:
    muduo::net::EventLoop m_eventloop;

    // 服务类型信息
    struct ServiceInfo
    {
        google::protobuf::Service *m_service; // 保存服务对象
        std::unordered_map < std::string, const google::protobuf::MethodDescriptor*> m_methodMap; // 保存服务方法
    };
    std::unordered_map<std::string, ServiceInfo> m_serviceMap;
    
    void OnConnection(const muduo::net::TcpConnectionPtr& conn);
    void OnMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf, muduo::Timestamp time);
    // closure的回调操作，用于序列化rpc的响应和网络发送
    void SendRpcResponse(const muduo::net::TcpConnectionPtr &conn, google::protobuf::Message*);
};