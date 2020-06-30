# include<unistd.h>
# include<stdio.h>
# include<string.h>
# include<time.h>
# include<assert.h>
# include<pthread.h>

pthread_cond_t cond;//
pthread_mutex_t mutex;//
char buff[128]={0};



void* fun(void* arg)
{
    char* s=(char*)arg;
    while(1)
    {
        pthread_mutex_lock(&mutex);//加锁
        pthread_cond_wait(&cond,&mutex);//阻塞，等待有条件发生
        pthread_mutex_unlock(&mutex);//解锁
        printf("%s:%s",s,buff);//
        if(strncmp(buff,"end",3)==0)
        {
            break;
        }
    }
}
int main()
{
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);

    pthread_t id[2];
    int res=pthread_create(&id[0],NULL,fun,"thread1");
    assert(res==0);
    res=pthread_create(&id[1],NULL,fun,"thread2");
    assert(res==0);

    while(1)
    {
        printf("input:");
        fgets(buff,127,stdin);
        if(strncmp(buff,"end",3)==0)//唤醒在条件变量cond上等待的所有线程
        {
            pthread_mutex_lock(&mutex);
            pthread_cond_broadcast(&cond);
            pthread_mutex_unlock(&mutex);
            break;
        }
        else//唤醒满足条件的一个线程
        {
            pthread_mutex_lock(&mutex);
            pthread_cond_signal(&cond);
            pthread_mutex_unlock(&mutex);
        }
        sleep(1);
    }
    pthread_join(id[0],NULL);
    pthread_join(id[1],NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}
