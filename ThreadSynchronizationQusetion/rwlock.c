# include<stdio.h>
# include<stdlib.h>
# include<unistd.h>
# include<assert.h>
# include<string.h>
# include<pthread.h>

int data=0;
pthread_rwlock_t rw;

void* reader(void* arg)
{
    int index=(int)(arg);
    while(1)
    {
        pthread_rwlock_rdlock(&rw);
        printf("---start read\n");
        printf("%d号读者，读出数据data=%d\n",index,data);
        printf("---end read\n");
        pthread_rwlock_unlock(&rw);
        sleep(1);
    }
}
void* writer(void* arg)
{
    int index=(int)(arg);
    while(1)
    {
        pthread_rwlock_wrlock(&rw);
        printf("---start write\n");
        data++;
        printf("%d号写者，写入数据data=%d\n",index,data);
        printf("---end write\n");
        pthread_rwlock_unlock(&rw);
        sleep(1);
    }
}

int main()
{
    pthread_rwlock_init(&rw,NULL);
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
    pthread_rwlock_destroy(&rw);
    exit(0);
}

