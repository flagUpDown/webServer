#include "./tool.h"

int createMySocket(){
    int socketfd;
    struct sockaddr_in myAddr;

    socketfd=socket(AF_INET,SOCK_STREAM,0);
    if(socketfd==-1){
        perror("socket: ");
        exit(EXIT_FAILURE);
    }

    myAddr.sin_family=AF_INET;
    myAddr.sin_port=htons(MYPORT);
    myAddr.sin_addr.s_addr=INADDR_ANY;
    bzero(&(myAddr.sin_zero),sizeof(myAddr.sin_zero));

    if(bind(socketfd,(struct sockaddr*)&myAddr,sizeof(struct sockaddr)) == -1){
        perror("bind: ");
        exit(EXIT_FAILURE);
    }

    if(listen(socketfd,BACKLOG) == -1){
        perror("listen: ");
        exit(EXIT_FAILURE);
    }

    return socketfd;
}

void process_conn_server(int connectfd){
    ssize_t size=0;
    char buffer[1024];
    char method[12],uri[120],version[12];

    size=read(connectfd,buffer,1024);
    sscanf(buffer,"%s %s %s",method,uri,version);
    char filename[150];
    sprintf(filename,"static%s",uri);
    int fd=open(filename,O_RDONLY);
    if(fd==-1){
        if(errno == ENOENT){
           httpResponse(buffer,"404 Not Found","text/plain","");
           loopWrite(connectfd,buffer,strlen(buffer));
        }
    }
    else{
        char filetype[30];
        getFileType(filename,filetype);
        httpResponse(buffer,"200 Ok",filetype,"");
        loopWrite(connectfd,buffer,strlen(buffer));

        struct stat st;
        size_t filesize;
        if(-1 == stat(filename,&st)){
            perror("stat: ");
            exit(EXIT_FAILURE);
        }
        else{
            filesize=st.st_size;
        }
        void* ptr=mmap(NULL,filesize,PROT_READ,MAP_PRIVATE,fd,0);
        if((char*)-1 == ptr){
            perror("mmap: ");
            exit(EXIT_FAILURE);
        }
        loopWrite(connectfd,ptr,filesize);
        munmap(ptr,filesize);
        close(fd);
    }
}

void httpResponse(char* str,char* statecode,char* type,char* add){
    sprintf(str,"HTTP/1.1 %s\r\n",statecode);
    strcat(str,"Server: fengjun web server\r\n");
    strcat(str,"Connection: close\r\n");
    sprintf(str,"%sContent-type: %s\r\n\r\n",str,type);
}

ssize_t loopWrite(int fd,void* buffer,size_t n){
    size_t nleft = n;
    ssize_t nwritten;
    char *cBuffer = buffer;

    while (nleft > 0) {
        if ((nwritten = write(fd,cBuffer,nleft)) <= 0){
            perror("write: ");
            exit(EXIT_FAILURE);
        }
        nleft-=nwritten;
        cBuffer+=nwritten;
    }
    return n;
}

void getFileType(char* filename,char* filetype){
    if(strstr(filename, ".html"))
        strcpy(filetype, "text/html");
    else if(strstr(filename,".css"))
        strcpy(filetype,"text/css");
    else if(strstr(filename, ".gif"))
        strcpy(filetype, "image/gif");
    else if(strstr(filename, ".png"))
        strcpy(filetype, "image/png");
    else if(strstr(filename, ".jpg"))
        strcpy(filetype, "image/jpeg");
    else
        strcpy(filetype, "text/plain");
}
