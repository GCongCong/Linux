# include<stdio.h>
# include<stdlib.h>
# include<string.h>
# include<unistd.h>
# include<assert.h>

# include<sys/types.h>
# include<pthread.h>
# include<sys/socket.h>
# include<arpa/inet.h>
# include<netinet/in.h>
# include<signal.h>
# include<semaphore.h>

#define THREAD_NUM 5//线程池的线程数量
#define BUFFSIZE 10 //文件描述符数量
sem_t sem;//信号量
pthread_mutex_t mutex; //互斥锁

int Buff_Fd[BUFFSIZE]; //线程共享。

//针对文件描述符的操作
//初始化
void InitBuffFd()
{
    int i=0;
    for(;i<BUFFSIZE;i++)
    {
        Buff_Fd[i]=-1;
    }
}

//插入,防止在插入时有另一个线程获取，所以加上互斥量
void InsertBuffFd(int fd)
{
    pthread_mutex_lock(&mutex);//加锁
    int i=0;
    for(;i<BUFFSIZE;++i)
    {
        if(Buff_Fd[i]==-1)
        {
            Buff_Fd[i]=fd;
            break;
        }
    }
    pthread_mutex_unlock(&mutex);//去锁

}
//获得文件描述符
int GetBuffFd()
{
    pthread_mutex_lock(&mutex);
    int i=0,fd=-1;
    for(;i<BUFFSIZE;++i)
    {
        if(Buff_Fd[i]!=-1)
        {
            fd=Buff_Fd[i];
            Buff_Fd[i]=-1;
            break;
        }
    }
    pthread_mutex_unlock(&mutex);
    return fd;
}
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
    if(res==-1) return -1;

    res=listen(sockfd,5);
    if(res==-1) return -1;

    return sockfd;
}
//线程工作
void* work_thread(void* arg)
{
    while(1)
    {
        //P操作阻塞它
        sem_wait(&sem);
        //获取文件描述符
        int fd=GetBuffFd();
        if(fd==-1) continue;
        //完成数据的收发
        while(1)
        {
            char buff[128]={0};
            int n=recv(fd,buff,127,0);
            if(n<=0)
            {
                printf("one client over\n");
                close(fd);
                break;
            }
            printf("%d:%s\n",fd,buff);
            int res=send(fd,"OK",2,0);
            if(res<=0)
            {
                printf("send error\n");
                close(fd);
                break;
            }
        }

    }
}
int main()
{
    //初始化信号量
    sem_init(&sem,0,0);
    //初始化互斥锁
    pthread_mutex_init(&mutex,NULL);
    //初始化描述符数组
   InitBuffFd();

   int sockfd=InitSocket();
   //创建线程池
   pthread_t id[THREAD_NUM];
   int i=0;
   for(;i<THREAD_NUM;++i)
   {
       int res=pthread_create(&id[i],NULL,work_thread,NULL);
       assert(res==0);
   }
   while(1)
   {
       struct sockaddr_in cli;
       socklen_t len=sizeof(cli);
       int c=accept(sockfd,(struct sockaddr*)&cli,&len);
       if(c<0)
       {
           continue;
       }
        printf("%d连接\n",c);
        //把C插入数组中
        InsertBuffFd(c);
        //启动一个线程去处理它,V操作
        sem_post(&sem);
   }
   close(sockfd);
   exit(0);

}
