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
        printf("i am child:pid=%d\n",getpid());
        execl("./test","./test","Hello","Linux",(char*)0);

        printf("child last  code\n");
    }
    else
    {
        printf("i am father\n");
        sleep(10);
        printf("father over");
    }
    exit(0);
}
