# include<stdio.h>
# include<string.h>
# include<unistd.h>
# include<assert.h>
# include<stdlib.h>
# include<pthread.h>

pthread_mutex_t mutex;
int G=0;

void* fun(void* arg)
{
    int i=0;
    for(;i<10000;++i)
    {
        pthread_mutex_lock(&mutex);
        G++;//
        pthread_mutex_unlock(&mutex);
    }

}
int main()
{
    pthread_mutex_init(&mutex,NULL);
    pthread_t id;
    int res=pthread_create(&id,NULL,fun,NULL);
    assert(res==0);
    int i=0;
    for(;i<10000;++i)
    {
        pthread_mutex_lock(&mutex);
        G++;
        pthread_mutex_unlock(&mutex);
    }
    pthread_join(id,NULL);
    printf("G=%d\n",G);
    exit(0);
}
