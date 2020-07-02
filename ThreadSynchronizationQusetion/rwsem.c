# include<stdio.h>
# include<stdlib.h>
# include<unistd.h>
# include<assert.h>
# include<string.h>
# include<pthread.h>
# include<semaphore.h>
int data=0;//临界资源
int count=0;//计数器，记录多少个读者
sem_t rw;//控制临界资源
sem_t mutex;//控制计数器


void* reader(void* arg)
{
    int index=(int)(arg);
    while(1)
    {
        sem_wait(&mutex);//获取计数器
        if(count==0)//第一个读者
        {
           
            sem_wait(&rw);//给临界资源加锁
            printf("first reader lock rw\n");
        }
        count++;//后面的读者不用加锁
        sem_post(&mutex);//释放计数器

        printf("%d号读者，读出数据data=%d\n",index,data);
      

        sem_wait(&mutex);
        count--;
        if(count==0)//最后一个读者解锁
        {
            sem_post(&rw);
            printf("last reader unlock rw\n");
        }
        sem_post(&mutex);
        sleep(1);
    }
}
void* writer(void* arg)
{
    int index=(int)(arg);
    while(1)
    {
        sem_wait(&rw);
        data++;
        printf("%d号写者，写入数据data=%d\n",index,data);
        sem_post(&rw);
        sleep(1);
    }
}

int main()
{
    sem_init(&rw,0,1);
    sem_init(&mutex,0,1);
    pthread_t id[5];
    int i=0;
    int res;
    for(;i<3;i++)
    {
        res=pthread_create(&id[i],NULL,reader,(void*)(i+1));
        assert(res==0);
    }
    for(;i<5;i++)
    {
        res=pthread_create(&id[i],NULL,writer,(void*)(i+1));
    }
    i=0;
    for(;i<5;i++)
    {
        pthread_join(id[i],NULL);
    }
    sem_destroy(&rw);
    sem_destroy(&mutex);
    exit(0);
}

