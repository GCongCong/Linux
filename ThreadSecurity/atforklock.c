# include<stdio.h>
# include<stdlib.h>
# include<unistd.h>
# include<string.h>
# include<assert.h>
# include<pthread.h>

pthread_mutex_t mutex;
void prepare()
{
    pthread_mutex_lock(&mutex);
}
void parent()
{
    pthread_mutex_unlock(&mutex);
}
void child()
{
    pthread_mutex_unlock(&mutex);
}
void* fun(void* arg)
{
    pid_t pid=fork();//锁为加锁状态
    assert(pid!=-1);
    if(pid==0)
    {
        printf("child,my pid=%d\n",getpid());
        pthread_mutex_lock(&mutex);//在加锁状态的锁上加锁会被阻塞，死锁
        printf("child lock sucess\n");
        sleep(2);
        pthread_mutex_unlock(&mutex);
        printf("child unlock success\n");
    }
    else
    {
        printf("father,my pid=%d\n",getpid());
        pthread_mutex_lock(&mutex);
        printf("father lock sucess\n");
        sleep(2);
        pthread_mutex_unlock(&mutex);
        printf("father unlock success\n");
    }
}

int main()
{
    pthread_mutex_init(&mutex,NULL);
    pthread_atfork(prepare,parent,child);
    printf("father pid=%d\n",getpid());
    pthread_t id;
    int res=pthread_create(&id,NULL,fun,NULL);
    assert(res==0);

    pthread_mutex_lock(&mutex);

    int i=0;
    for(;i<5;i++)
    {
        printf("main,my pid=%d\n",getpid());
        sleep(1);
    }
    pthread_mutex_unlock(&mutex);
    printf("main unlock success\n");
    pthread_exit(NULL);
    
}
