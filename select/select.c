# include<stdio.h>
# include<stdlib.h>
# include<unistd.h>
# include<string.h>
# include<assert.h>
//网络头文件
# include<sys/types.h>
# include<sys/socket.h>
# include<arpa/inet.h>
# include<netinet/in.h>
# include<sys/select.h>
#define MAX 10//定义最大连接数为10个
//用数组来记录用户的描述符号,所以要写数组初始化，数组删除，添加
void fds_init(int fds[])//先把数组全置为-1
{
    int i=0;
    for(;i<MAX;i++)
    {
        fds[i]=-1;
    }
}
void fds_add(int fds[],int fd)//添加事件描述符
{
    int i=0;
    for(;i<MAX;i++)
    {
        if(fds[i]==-1)
        {
            fds[i]=fd;
            break;
        }
    }
}
void fds_del(int fds[],int fd)//删除描述符
{
    int i=0;
    for(;i<MAX;i++)
    {
        if(fds[i]==fd)
        {
            fds[i]=-1;
            break;
        }
    }
}

int InitSocket()
{
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd == -1) return -1;
    struct sockaddr_in ser;//指明地址信息，是一种通用的套接字地址
    memset(&ser,0,sizeof(ser));
    ser.sin_family = AF_INET;//设置地址家族
    ser.sin_port = htons(6000);//设置端口
    ser.sin_addr.s_addr = inet_addr("127.0.0.1");//设置地址
    int res = bind(sockfd,(struct sockaddr*)&ser,sizeof(ser));//绑定端口号和地址
    if(res == -1)   return -1;
    res = listen(sockfd,5);
    if(res == -1) return -1;
    return sockfd;
}
// 处理就绪事件函数
void DealFinishEvent(fd_set readfds,int sockfd,int fds[])
{
    int i=0;
    for(;i<MAX;i++)//表示一直处理就绪事件
    {
        if(fds[i]==-1)
        {
            continue;
        }

        if(FD_ISSET(fds[i],&readfds))//判断事件描述符是否就绪
        {
            if(fds[i]==sockfd)//有新的客户端链接，只有两种套接字：监听和链接,sockfd为链接的
            {
                struct sockaddr_in caddr;//
                int len=sizeof(caddr);
                int c=accept(sockfd,(struct sockaddr*)&caddr,&len);//接受链接
                //将c添加到用户数组描述符中
                if(c<0)//接收失败
                {
                    continue;
                }
                printf("accept c=%d\n",c);
                fds_add(fds,c);
            }
            else//表示为监听套接字，有数据到达
            {
                //接受数据
                char buff[128]={0};//把从客户端接受的数据保存到buff里
                if(recv(fds[i],buff,127,0)<=0)//表示没有数据了
                {
                    close(fds[i]);
                    printf("one client over\n");
                    fds_del(fds,fds[i]);

                }
                else//给客户端一个回应
                {
                    printf("read:%s\n",buff);
                    send(fds[i],"ok",2,0);//给客户端发送ok

                }
            
      }

        }
    }
}
int main()
{
    int sockfd = InitSocket();//监听套接字，有客户端链接时就会触发读事件。
    assert(sockfd != -1);
    //将sockfd添加到数组中
    int fds[MAX];
    fds_init(fds);
    fds_add(fds,sockfd);

    fd_set readfds;//定义保存可读事件结构体，大小为1024位

    while(1)
    {
       //因为在线修改的性质，所以每次都要重新设置readfds的值
       FD_ZERO(&readfds);//数据清空
       int maxfd=-1;
       int i=0;
       for(;i<MAX;i++)
       {
           if(fds[i]==-1)
           {
               continue;
           }
           FD_SET(fds[i],&readfds);//重新设置值
           if(maxfd<fds[i])
           {
               maxfd=fds[i];
           }
       }
       struct timeval tv={6,0};
       int n=select(maxfd+1,&readfds,NULL,NULL,&tv);//在这我们只测试可读事件
       if(n<=0)//-1表示select失败，没有就绪事件0表示超时
       {
           printf("select error\n");
           continue;
       }
       DealFinishEvent(readfds,sockfd,fds);//处理就绪的事件，需要传入readfds在线修改他里面保存着就绪事件描述符
    }
    close(sockfd);
}
