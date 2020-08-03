# include<stdio.h>
# include<stdlib.h>
# include<unistd.h>
# include<assert.h>

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
        printf("i am father\n");
        sleep(20);
        printf("father over");
    }
    exit(0);
}
