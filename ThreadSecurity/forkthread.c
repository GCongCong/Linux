# include<stdio.h>
# include<stdlib.h>
# include<unistd.h>
# include<string.h>
# include<assert.h>
# include<pthread.h>

void* fun(void* arg)
{
    pid_t pid=fork();
    assert(pid!=-1);
    if(pid==0)
    {
        printf("child,my pid=%d\n",getpid());
    }
    else
    {
        printf("father,my pid=%d\n",getpid());
    }
}

int main()
{
    printf("father pid=%d\n",getpid());
    pthread_t id;
    int res=pthread_create(&id,NULL,fun,NULL);
    assert(res==0);

    sleep(1);

    int i=0;
    for(;i<5;i++)
    {
        printf("main,my pid=%d\n",getpid());
        sleep(1);
    }
}
