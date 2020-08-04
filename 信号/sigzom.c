# include<stdio.h>
# include<stdlib.h>
# include<assert.h>
# include<string.h>
# include<signal.h>
# include<unistd.h>

void dealzom(int sign)
{
    printf("deal child\n");
    wait(NULL);
}

int main()
{
    pid_t pid=fork();
    if(pid==0)
    {
        printf("i am child\n");
        sleep(5);
        printf("child bye\n");
    }
    else
    {
        printf("i am father\n");
        signal(SIGCHLD,dealzom);
        sleep(20);
        printf("father over\n");
    }
    exit(0);
}
