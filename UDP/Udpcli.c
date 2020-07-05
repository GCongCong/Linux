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

    struct sockaddr_in ser_addr;//定义服务端结构体，客户端需要先给服务端发信息
    memset(&ser_addr,0,sizeof(ser_addr));
    ser_addr.sin_family=AF_INET;
    ser_addr.sin_port=htons(6000);
    ser_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    while(1)
    {

        char buff[128]={0};
        printf("input:");
        fgets(buff,127,stdin);
        if(strncmp(buff,"end",3)==0)
        {
            break;
        }
        int res=sendto(sockfd,buff,strlen(buff)-1,0,(struct sockaddr*)&ser_addr,sizeof(ser_addr));
        assert(res!=-1);
        memset(buff,0,128);
        int n=recvfrom(sockfd,buff,127,0,NULL,NULL);//因为已经和服务器建立连接，所以不用指定了，使用默认即可
        printf("recvfrom ser data:%s\n",buff);
    }
    close(sockfd);
    exit(0);
}
