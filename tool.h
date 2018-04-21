#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/socket.h>

#define MYPORT 6699
#define BACKLOG 5

//创建socket连接
//返回socket描述符
int createMySocket();
//通过套接字文件描述符，响应浏览器请求
void process_conn_server(int);
//设置响应头
void httpResponse(char* ,char* ,char*,char*);
//将buffer里的内容写进对应的文件描述符里
ssize_t loopWrite(int ,void* ,size_t);
//通过文件名获得响应的文件类型
void getFileType(char* ,char*);
