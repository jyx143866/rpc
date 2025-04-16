/*
 * @Author: jyx
 * @Date: 2025-04-16 05:54:43
 * @LastEditors: jyx
 * @Description:
 */
#include "mprpccontroller.h"

MprpcController::MprpcController()
    : m_failed(false), m_errText("")
{
    
}

void MprpcController::Reset()
{
    m_failed = false;
    m_errText = "";
}
bool MprpcController::Failed() const
{
    return m_failed;
}
std::string MprpcController::ErrorText() const
{
    return m_errText;
}
void MprpcController::SetFailed(const std::string &reason)
{
    m_failed = true;
    m_errText = reason;
}

// 目前未实现具体的功能
void MprpcController::StartCancel()
{}
bool MprpcController::IsCanceled() const
{
    return false;
}
void MprpcController::NotifyOnCancel(google::protobuf::Closure *callback)
{}