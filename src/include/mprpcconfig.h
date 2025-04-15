/***
 * @Author: jyx
 * @Date: 2025-04-10 09:06:23
 * @LastEditors: jyx
 * @Description:
 */
#pragma once

#include <unordered_map>
#include <string>

// 框架配置文件类rpcserverip,port    zookeeperip, port
class MprpcConfig{
public:
    // 负责解析加载配置文件
    void LoadConfigFile(const char* config_file);
    // 查询配置文件信息
    std::string Load(const std::string &key);
private:
    std::unordered_map<std::string, std::string> m_configMap;
    // 去掉字符串前后的空格
    void Trim(std::string &src_buf);
};