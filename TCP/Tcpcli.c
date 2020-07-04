# include<stdio.h>
# include<stdlib.h>
# include<unistd.h>
# include<assert.h>
# include<sys/types.h>
# include<sys/socket.h>
# include<netinet/in.h>
# include<arpa/inet.h>
# include<string.h>

int main()
{
    int sockfd=socket(AF_INET,SOCK_STREAM,0);//创建套接字
    assert(sockfd!=-1);

    //客户端必须定义服务器的，否则无法连接
    struct sockaddr_in ser_addr;//定义服务器TCP/IP专用socket地址
    memset(&ser_addr,0,sizeof(ser_addr));//置为空，防止下一个客户端无法连接
    ser_addr.sin_family=AF_INET;//设置地址族
    ser_addr.sin_port=htons(6000);//设置端口号，将主机字节序转换为网络字节序
    ser_addr.sin_addr.s_addr=inet_addr("127.0.0.1");//设置IPv4,将点分十进制转换为网络字节序整数表示的IPV4地址
    
    int res=connect(sockfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));//连接服务器
    assert(res!=-1);
    //循环接受客户端连接，
    while(1)
    {
        printf("please input:");
        char data[128]={0};
        fgets(data,127,stdin);
        if(strncmp(data,"bye",3)==0)
        {
            break;
        }
        int num=send(sockfd,data,strlen(data)-1,0);
        assert(num!=-1);
        if(num==0)
        {
            printf("send length is zero\n");
            break;
        }
        char buff[128]={0};
        int n=recv(sockfd,buff,127,0);
        assert(n!=-1);
        if(n==0)
        {
            printf("error\n");
            break;
        }
        printf("recv ser data is:%s\n",buff);
    }
    close(sockfd);//关闭服务器
    exit(0);
}
