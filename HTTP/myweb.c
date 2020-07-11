# include<stdio.h>
# include<stdlib.h>
# include<assert.h>
# include<string.h>
# include<unistd.h>
//网络头文件
# include<sys/types.h>
# include<sys/socket.h>
# include<arpa/inet.h>
# include<netinet/in.h>
//文件头文件
# include<fcntl.h>
# include<sys/stat.h>
int InitSocket()//socket().bind(),listen()完成就可以接受客户端连接了
{
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1)
    {
        return -1;
    }
    struct sockaddr_in ser;
    memset(&ser,0,sizeof(ser));
    ser.sin_family=AF_INET;
    ser.sin_port=htons(80);//http默认端口80
    ser.sin_addr.s_addr=inet_addr("127.0.0.1");
    int res=bind(sockfd,(struct sockaddr*)&ser,sizeof(ser));
    if(res==-1)
    {
        return -1;
    }
    res=listen(sockfd,5);//在内核创建两个队列：已经完成三次握手，正在进行三次握手，5指定的就是已经完成三次握手队列的大小，则意味着，如果当前程序不能及时处理这个链接，那么它最多维护5+1个客户端链接
    if(res==-1)
    {
        return -1;
    }
    return sockfd;

}
//解析收到的数据
int  RecvData(int c,char *file_name)
{
    char buff[1024]={0};
    int n=recv(c,buff,1023,0);
    if(n<=0)
    {
        return -1;
    }
    printf("%s\n",buff);
    char *p=strtok(buff," ");//-->得到第一个参数，请求方法get
    p=strtok(NULL," ");//-->得到客户端申请的文件名
    char path[128]="/var/www/html/";//定义文件存储位置，确保在绝对路径下查找文件
    strcat(path,p);//把路径和文件名连接
    //用open方法打开文件来判断文件是否存在,-1表示失败，大于0表示正确
    int fd=open(path,O_RDONLY);
    if(fd==-1)//表示不存在此文件，我们就把404文件路径给file_name，表示找不到
    {
        strcpy(file_name,"/var/www/html/404.html");
    }
    else//表示找到，把绝对路径拷贝到file_name中
    {
        strcpy(file_name,path);
        close(fd);
    }
    return 0;
}
void SendHead(int c,int flag,int size)
{
    char buff[1024]={0};
    strcat(buff,"HTTP/1.0");//应答报文第一行版本号
    if(flag)//根据标志判断状态吗
    {
        strcat(buff,"404 NOt Found\r\n");
    }
    else
    {
        strcat(buff,"200 ok\r\n");
    }
    //第二行服务器名称
    strcat(buff,"Server: MYWEB/1.0\r\n");
    //第三行数据长度
    strcat(buff,"Content-Length:");
    //把数字连接到字符串
    sprintf(buff+strlen(buff),"%d",size);
    strcat(buff,"\r\n");
    //第四行文件类型
    strcat(buff,"Content-Type:text/html;charset=utf-8\r\n");
    //第五行为分割头部和数据的空行
    strcat(buff,"\r\n");
    send(c,buff,strlen(buff),0);
    
}
void SendData(int c,char* file_name)
{
    int flag=0;//标志404还是200
    if(strstr(file_name,"404.html")!=NULL)//判断文件路径是否包含404.html字符
    {
        flag=1;
    }
    struct stat st;//保存文件内容的结构体
    stat(file_name,&st);//把file_name路径下的文件放进去
    SendHead(c,flag,st.st_size);//成员包含文件内容长度
    int fd=open(file_name,O_RDONLY);//打开对应路径下的文件，返回值为文件描述符
    while(1)
    {
        char buff[128]={0};
        int n=read(fd,buff,127);//读取数据
        if(n<=0)
        {
            break;
        }
        send(c,buff,n,0);//发送
    }
    close(fd);

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
            printf("accept error");
            continue;
        }
        char file_name[128]={0};//保存接受请求报文
        int num=RecvData(c,file_name);//接受请求报文头信息，进行解析，将解析的文件存储在file_name中
        if(num==-1)
        {
            close(c);
            continue;
        }
        SendData(c,file_name);//发送应答信息
        close(c);//短连接，在发送数据后就断开
    }
 }
