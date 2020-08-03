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
        sleep(5);
        printf("child over\n");
    }
    else
    {
        pid_t id;
        do//必须循环
        {
            id=waitpid(pid,NULL,WNOHANG);
            if(id==0)
            {
                printf("child run\n");
                sleep(1);
            }
        }while(id==0);

        printf("i am father\n");
        sleep(10);
        printf("father over");
    }
    exit(0);
}
