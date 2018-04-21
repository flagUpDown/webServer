#include "./tool.h"

int main()
{
    int socketfd=createMySocket();
    struct sockaddr_in client_addr;
    while(1){
        //接受网络请求，返回套接字文件描述符
        socklen_t addrlen=sizeof(struct sockaddr);
        int connectfd=accept(socketfd,(struct sockaddr*)&client_addr,&addrlen);
        if(connectfd<0){
            perror("accept: ");
        }
        process_conn_server(connectfd);
        close(connectfd);
    }
    close(socketfd);

    return 0;
}
