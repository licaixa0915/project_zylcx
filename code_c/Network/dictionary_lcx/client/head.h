#ifndef __HEAD_H__
#define __HEAD_H__

//消息的类型
#define USER_REGISTER    10
#define USER_LOGIN       20
#define USER_WORD        30
#define USER_SUCCESS     40
#define USER_FAILURE     50

typedef struct
{
    char _username[32]; //用户名
    char _password[32]; //密码
}USER_T;
//__attribute__((__packed__))user_t;

typedef struct
{
    int type;           //消息类型
    int size;           //消息大小
    union
    {
        USER_T uinfo;       //用户信息
        char   _word[128]; 
    }content;

    //客户端填单词，服务端填写单词解释
    #define word         content._word
    #define username     content.uinfo._username
    #define password       content.uinfo._password
}MHEAD_T;
//__attribute__((__packed__))mhead_t;
typedef struct 
{
    int msgType;
    int retType;
    char result[1024];
}SHEAD_T;
#endif
