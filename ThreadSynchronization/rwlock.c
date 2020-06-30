# include<unistd.h>
# include<stdio.h>
# include<string.h>
# include<time.h>
# include<assert.h>
# include<pthread.h>

pthread_rwlock_t rwlock;//
char buff[128]={0};



void* fun(void* arg)
{
    while(1)
    {
        pthread_rwlock_rdlock(&rwlock);
        if(strncmp(buff,"end",3)==0)
        {
            break;
        }
        printf("fun:%s\n",buff);
        //memset(buff,0,128);
        
        int n=rand()%3+1;
        sleep(n);

        pthread_rwlock_unlock(&rwlock);
        n=rand()%3+1;
        sleep(1);
    }
}
void* fun1(void* arg)
{
    while(1)
    {
        pthread_rwlock_rdlock(&rwlock);
        if(strncmp(buff,"end",3)==0)
        {
            break;
        }
        printf("fun1:%s\n",buff);
        //memset(buff,0,128);
        
        int n=rand()%3+1;
        sleep(n);

        pthread_rwlock_unlock(&rwlock);
        n=rand()%3+1;
        sleep(1);
    }
}
int main()
{
    srand((unsigned int)(time(NULL)*time(NULL)));
    pthread_rwlock_init(&rwlock,NULL);//
    pthread_t id[2];
    int res=pthread_create(&id[0],NULL,fun,NULL);
    int r=pthread_create(&id[1],NULL,fun1,NULL);
    assert(res==0);

    while(1)
    {
        pthread_rwlock_wrlock(&rwlock);
        printf("input:");
        fgets(buff,127,stdin);
        pthread_rwlock_unlock(&rwlock);
        if(strncmp(buff,"end",3)==0)
        {
            break;
        }
        int n=rand()%3+1;
        sleep(1);
    }
    
    pthread_join(id[0],NULL);
    pthread_join(id[1],NULL);
    pthread_rwlock_destroy(&rwlock);
}
