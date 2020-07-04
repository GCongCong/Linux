# include<stdio.h>
# include<stdlib.h>
# include<unistd.h>
# include<assert.h>
# include<string.h>
# include<sys/types.h>
# include<sys/socket.h>
# include<netinet/in.h>
# include<arpa/inet.h>


int main()
{
    int listenfd=socket(AF_INET,SOCK_STREAM,0);//创建套接字
    assert(listenfd!=-1);

    struct sockaddr_in ser_addr;//定义服务器TCP/IP专用socket地址
    memset(&ser_addr,0,sizeof(ser_addr));//置为空，防止下一个客户端无法连接
    ser_addr.sin_family=AF_INET;//设置地址族
    ser_addr.sin_port=htons(6000);//设置端口号，将主机字节序转换为网络字节序
    ser_addr.sin_addr.s_addr=inet_addr("127.0.0.1");//设置IPv4,将点分十进制转换为网络字节序整数表示的IPV4地址
    int res=bind(listenfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));//命名绑定socket
    assert(res!=-1);

    res=listen(listenfd,5);//监听，监听队列为5
    assert(res!=-1);

    //循环接受客户端连接，
    while(1)
    {
        struct sockaddr_in cli_addr;//定义客户端socket地址
        socklen_t len=sizeof(cli_addr);

        int clientfd=accept(listenfd,(struct sockaddr*)&cli_addr,&len);//接收一个客户端
        if(clientfd==-1)
        {
            printf("one client link error\n");
            continue;
        }
        printf("one client success--%s:%d\n",inet_ntoa(cli_addr.sin_addr),ntohs(cli_addr.sin_port));
        //循环接受，发送数据
        while(1)
        {
            char buff[128]={0};
            int num=recv(clientfd,buff,127,0);//接收数据
            if(num==-1)//接收失败
            {
                printf("recv error\n");
                break;
            }
            else if(num==0)//客户端关闭
            {
                printf("client over\n");
                break;
            }
            printf("recv data is:%s\n",buff);

            char* restr="recv data success";
            num=send(clientfd,restr,strlen(restr),0);//回复数据
            if(num==-1)
            {
                printf("send data error\n");
                break;
            }
        }
        close(clientfd);//关闭客户端连接，服务器还可以接收下一个客户端
    }
    close(listenfd);//关闭服务器
    exit(0);
}
