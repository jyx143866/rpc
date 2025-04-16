/***
 * @Author: jyx
 * @Date: 2025-04-15 05:42:21
 * @LastEditors: jyx
 * @Description:
 */
#pragma once
#include "zookeeperutil.h"
#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>

class MprpcChannel : public google::protobuf::RpcChannel{
public:
    void CallMethod(const google::protobuf::MethodDescriptor *method,
                    google::protobuf::RpcController *controller,
                    const google::protobuf::Message *request,
                    google::protobuf::Message *response,
                    google::protobuf::Closure *done);
};