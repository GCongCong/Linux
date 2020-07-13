# include<stdio.h>
# include<stdlib.h>
# include<string.h>
# include<unistd.h>
# include<assert.h>

# include<pthread.h>
# include<sys/types.h>
# include<sys/socket.h>
# include<arpa/inet.h>
# include<netinet/in.h>

int InitSocket()
{
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==1) return -1;

    struct sockaddr_in ser;
    memset(&ser,0,sizeof(ser));
    ser.sin_family=AF_INET;
    ser.sin_port=htons(6000);
    ser.sin_addr.s_addr=inet_addr("127.0.0.1");
    int res=bind(sockfd,(struct sockaddr*)&ser,sizeof(ser));
    if(res==-1) return -1;

    res=listen(sockfd,5);
    if(res==-1) return -1;

    return sockfd;
}
//处理数据的线程
void*  work_thread(void* arg)
{
    int c=(int)arg;
    while(1)
    {
        char buff[128]={0};
        int n=recv(c,buff,127,0);
        if(n<=0)
        {
            break;
        }
        printf("%d:%s\n",c,buff);
        send(c,"ok",2,0);
    }
    close(c);//一定要记得关闭，不然资源会浪费
}
int main()
{
    int sockfd=InitSocket();
    assert(sockfd!=-1);
    while(1)
    {
        struct sockaddr_in cli;
        socklen_t len=sizeof(cli);
        int c=accept(sockfd,(struct sockaddr*)&cli,&len);
        if(c<0)
        {
            continue;
        }
        pthread_t id;
        int res=pthread_create(&id,NULL,work_thread,(void*)c);//线程接口，最后一个元素为void*类型，所以要强转c
    }
    close(sockfd);
}
