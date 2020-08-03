# include<stdio.h>
# include<stdlib.h>
# include<unistd.h>
# include<assert.h>
# include<sys/wait.h>

int main()
{
    pid_t pid=fork();
    assert(pid!=-1);
    if(pid==0)
    {
        printf("i am child\n");
        sleep(10);
        printf("child over\n");
    }
    else
    {
        pid_t id=wait(NULL);
        printf("i am father\n");
        sleep(20);
        printf("father over");
    }
    exit(0);
}
