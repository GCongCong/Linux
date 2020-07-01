# include<stdio.h>
# include<assert.h>
# include<unistd.h>
# include<stdlib.h>
# include<pthread.h>
# include<semaphore.h>
# include<string.h>

int buff[5]={0};//缓冲区
sem_t mutex;//控制缓冲区
sem_t empty;//控制空格子数
sem_t full;//控制有数据格子

int i=0;
int j=0;


void* producer(void* argc)
{
    while(1)
    {
        int index=(int)(argc);
        srand(time(NULL));//生成随机数字
        int data=rand()%11;
        sem_wait(&empty);
        sem_wait(&mutex);
        buff[i]=data;
        printf("生产者线程%d往buff%d下标添加数字%d\n",index,i,data);
        i=(i+1)%5;
        sem_post(&full);
        sem_post(&mutex);
        sleep(1);
    }
}
void* consumer(void* argc)
{
    while(1)
    {
        int index=(int)(argc);
        srand(time(NULL));
        int data=rand()%11;
        sem_wait(&full);
        sem_wait(&mutex);
        printf("消费者线程%d从buff第%d取出数字%d\n",index,j,buff[j]);
        buff[j]=0;
        j=(j+1)%5;
        sem_post(&empty);
        sem_post(&mutex);
        sleep(1);
    }
}


int main()
{
    pthread_t id[4];
    sem_init(&mutex,0,1);
    sem_init(&empty,0,5);
    sem_init(&full,0,0);
    int i=0;
    for(;i<2;i++)//创建生产者
    {
        pthread_create(&id[i],NULL,producer,(void*)(i+1));
    }
    for(;i<4;i++)//创建消费者
    {
        pthread_create(&id[i],NULL,consumer,(void*)(i+1));

    }
    pthread_join(id[0],NULL);
    pthread_join(id[1],NULL);
    pthread_join(id[2],NULL);
    pthread_join(id[3],NULL);
    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    exit(0);
}
