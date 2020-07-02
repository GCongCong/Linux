# include<stdio.h>
# include<stdlib.h>
# include<unistd.h>
# include<assert.h>
# include<string.h>
# include<pthread.h>
# include<semaphore.h>

sem_t sem[5];//信号量数组，控制5根筷子
sem_t mutex;//控制是否进入分筷子状态

void* eat(void* arg)
{
    int i=(int)(arg);
   // while(1)加上循环表示5位哲学家不断地需要进行吃饭，不加就是一轮吃饭
   // {
        sem_wait(&mutex);//进入分筷子状态

        sem_wait(&sem[i]);//分左边筷子
        sem_wait(&sem[(i+1)%5]);//分右边

        sem_post(&mutex);//分筷子结束，退出状态
        printf("哲学家%d在吃饭\n",i+1);//吃饭饭

        sem_post(&sem[i]);//放下筷子
        sem_post(&sem[(i+1)%5]);
   // }

}

int main()
{
    int i=0;
    for(;i<5;i++)
    {
        sem_init(&sem[i],0,1);//初始化信号量数组
    }
    sem_init(&mutex,0,1);

    pthread_t id[5];
    i=0;
    for(;i<5;i++)
    {
        pthread_create(&id[i],NULL,eat,(void*)(i));//创建5个哲学家
    }

    i=0;
    for(;i<5;i++)
    {
        pthread_join(id[i],NULL);
    }
    i=0;
    for(;i<5;i++)
    {
        sem_destroy(&sem[i]);
    }
    sem_destroy(&mutex);
    exit(0);
}
