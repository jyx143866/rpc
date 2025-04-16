/***
 * @Author: jyx
 * @Date: 2025-04-16 05:54:30
 * @LastEditors: jyx
 * @Description:
 */
#pragma once
#include <string>
#include <google/protobuf/service.h>

class MprpcController : public google::protobuf::RpcController
{
public:
    MprpcController();
    void Reset();
    bool Failed() const;
    std::string ErrorText() const;
    void SetFailed(const std::string &reason);

    // 目前未实现具体的功能
    void StartCancel();
    bool IsCanceled() const;
    void NotifyOnCancel(google::protobuf::Closure* callback);
private:
    bool m_failed;
    std::string m_errText;
};