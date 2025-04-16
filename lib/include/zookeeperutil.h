/***
 * @Author: jyx
 * @Date: 2025-04-16 16:04:28
 * @LastEditors: jyx
 * @Description:
 */
#pragma once

#include <semaphore.h>
#include <zookeeper/zookeeper.h>
#include <string>

class ZkClient
{
public:
    ZkClient();
    ~ZkClient();
    // zkclient启动连接zkserver
    void start();
    // 在zkserver上根据指定的path创建znode节点
    void Create(const char *path, const char *data, int datalen, int state = 0);
    // 根据传入参数路径，获取znode的值
    std::string GetData(const char* path);
private:
    zhandle_t *m_zhandle;
};