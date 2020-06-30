# include<unistd.h>
# include<stdio.h>
# include<string.h>
# include<time.h>
# include<assert.h>
# include<pthread.h>
# include<semaphore.h>
sem_t sem1;
sem_t sem2;
char buff[128]={0};



void* fun(void* arg)
{
    while(1)
    {
        sem_wait(&sem1);//P
        if(strncmp(buff,"end",3)==0)
        {
            break;
        }
        printf("fun:%s\n",buff);
        memset(buff,0,128);
        
        int n=rand()%3+1;
        sleep(n);

        sem_post(&sem2);//V
        n=rand()%3+1;
        sleep(1);
    }
}
int main()
{
    srand((unsigned int)(time(NULL)*time(NULL)));
    sem_init(&sem1,0,0);
    sem_init(&sem2,0,1);
    pthread_t id;
    int res=pthread_create(&id,NULL,fun,NULL);
    assert(res==0);

    while(1)
    {
        sem_wait(&sem2);//P
        printf("input:");
        fgets(buff,127,stdin);
        sem_post(&sem1);//V
        if(strncmp(buff,"end",3)==0)
        {
            break;
        }
        int n=rand()%3+1;
        sleep(1);
    }
    pthread_join(id,NULL);
    sem_destroy(&sem1);
    sem_destroy(&sem2);    
}
