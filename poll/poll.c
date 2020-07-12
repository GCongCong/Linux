# define  _GNU_SOURCE//用来引入客户端断开链接事件,要放到最上面
# include<stdio.h>
# include<stdlib.h>
# include<unistd.h>
# include<string.h>
# include<assert.h>

# include<sys/types.h>
# include<sys/socket.h>
# include<arpa/inet.h>
# include<netinet/in.h>
# include<poll.h>

# define LEN 100

int InitSocket()
{
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1) return -1;
    struct sockaddr_in ser;
    memset(&ser,0,sizeof(ser));
    ser.sin_family=AF_INET;
    ser.sin_port=htons(6000);
    ser.sin_addr.s_addr=inet_addr("127.0.0.1");

    int res=bind(sockfd,(struct sockaddr*)&ser,sizeof(ser));
    if(res==-1)return -1;

    res=listen(sockfd,5);
    if(res==-1) return -1;

    return sockfd;
}

void InitFds(struct pollfd *fds)
{
    int i=0;
    for(;i<LEN;i++)
    {
        fds[i].fd=-1;//表示用户事件描述符初始化为-1
        fds[i].events=0;//事件类型初始化为0
    }
}
InsertFd(struct pollfd *fds,int fd,short events)//插入描述符
{
    int i=0;
    for(;i<LEN;i++)
    {
        if(fds[i].fd==-1)//表示这个位置没有事件，可以插入
        {
            fds[i].fd=fd;
            fds[i].events=events;
            break;
        }
    }
}
void DealFinishEvent(struct pollfd *fds,int sockfd)
{
    int i=0;
    for(;i<LEN;i++)
    {
        if(fds[i].fd==-1)//表示没有就绪事件
        {
            continue;
        }
        else if(fds[i].fd==sockfd)//表示监听描述符，有一个新的链接过来了
        {
            if(fds[i].revents&POLLIN)//判断是否有事件就绪，为1表示revents上有POLLIN事件了
            {
                struct sockaddr_in cli;
                socklen_t len=sizeof(cli);
                int c=accept(sockfd,(struct sockaddr*)&cli,&len);
                if(c<0)
                {
                    printf("accept error\n");
                    continue;
                }
                printf("one client link\n");
                InsertFd(fds,c,POLLIN | POLLRDHUP);//添加描述符到结构体上去
            }
        }
        else//客户端链接文件描述符号
        {
            //没有事件发生，啥也不做
            //POLLRDHUP事件发生客户端断开链接
            if(fds[i].revents& POLLRDHUP)
            {
                printf("one client over\n");
                close(fds[i].fd);
                fds[i].fd=-1;
                fds[i].events=0;
            }
            //POLLIN事件发生，表示接收到了数据
            else if(fds[i].revents& POLLIN)
            {
                char buff[128]={0};
                int n=recv(fds[i].fd,buff,127,0);
                if(n<=0)
                {
                    printf("recv error\n");
                    continue;
                }
                printf("%d:%s\n",fds[i].fd,buff);
                send(fds[i].fd,"ok",2,0);
            }

        }
    }
}
int main()
{
    int sockfd=InitSocket();
    assert(sockfd!=-1);
    struct pollfd fds[LEN];//给poll传递的结构体，3部分构成
    InitFds(fds);
    InsertFd(fds,sockfd,POLLIN);//向结构体中插入事件描述符，事件类型（可读）
    while(1)
    {
        int n=poll(fds,LEN,-1);//永久阻塞
        if(n<=0)
        {
            printf("poll error\n");
            continue;
        }
        DealFinishEvent(fds,sockfd);
    }
    close(sockfd);
    exit(0);
}
