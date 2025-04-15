/*
 * @Author: jyx
 * @Date: 2025-04-10 05:55:46
 * @LastEditors: jyx
 * @Description: 
 */
#include "test.pb.h"
#include <iostream>
#include <string>    
using namespace fixbug;

int main(){
    
    LoginResponse rsp;
    ResultCode *rc = rsp.mutable_result();
    rc->set_errcode(1);
    rc->set_errmsg("登录处理失败了");

    GetFriendListResponse rsp1;
    ResultCode *rc1 = rsp.mutable_result();
    rc1->set_errcode(0);
    User *user1 = rsp1.add_friend_list();
    user1->set_name("zhang san");
    user1->set_age(18);
    user1->set_sex(User::MAN);

    User *user2 = rsp1.add_friend_list();
    user1->set_name("zhang san");
    user1->set_age(20);
    user1->set_sex(User::MAN);

    std::cout << rsp1.friend_list_size() << std::endl;

    return 0;
}

int main1(){
    // 封装了login请求对象的数据
    LoginRequest req;
    req.set_name("zhang san");
    req.set_pwd("123456");

    // 对象数据序列化
    std::string send_str;
    if(req.SerializeToString(&send_str)){
        std::cout << send_str.c_str() << std::endl;
    }

    // 从send_str反序列化一个login请求对象
    LoginRequest reqb;
    if(reqb.ParseFromString(send_str)){
        std::cout << reqb.name() << std::endl;
        std::cout << reqb.pwd() << std::endl;
    }

    return 0;
}