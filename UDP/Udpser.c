# include<stdio.h>
# include<stdlib.h>
# include<string.h>
# include<assert.h>
# include<unistd.h>
# include<sys/socket.h>
# include<sys/types.h>
# include<arpa/inet.h>
# include<netinet/in.h>


int main()
{
    int sockfd=socket(PF_INET,SOCK_DGRAM,0);
    assert(sockfd!=-1);

    struct sockaddr_in ser_addr;
    memset(&ser_addr,0,sizeof(ser_addr));
    ser_addr.sin_family=AF_INET;
    ser_addr.sin_port=htons(6000);
    ser_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    int res=bind(sockfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));//需要绑定
    assert(res!=-1);
    while(1)
    {
        struct sockaddr_in cli_addr;//获取客户端的地址信息，存储在这里
        socklen_t len=sizeof(cli_addr);
        memset(&cli_addr,0,sizeof(cli_addr));

        char buff[128]={0};
        int n=recvfrom(sockfd,buff,127,0,(struct sockaddr*)&cli_addr,&len);
        if(n<=0)
        {
            printf("recvfrom error\n");
            continue;
        }
        printf("%s:%d--->%s\n",inet_ntoa(cli_addr.sin_addr),ntohs(cli_addr.sin_port),buff);
        int res=sendto(sockfd,"ok",2,0,(struct sockaddr*)&cli_addr,len);
        if(res<=0)
        {
            printf("sendto error\n");
            continue;
        }
    }
    close(sockfd);
    exit(0);
}
