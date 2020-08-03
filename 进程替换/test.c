# include<stdio.h>
# include<unistd.h>
# include<assert.h>
# include<stdlib.h>

int main(int argc,char* argv[])
{
    printf("i am child new code:my pid=%d\n",getpid());
    int i=0;
    for(;i<argc;i++)
    {
        printf("argv[%d]=%s\n",i,argv[i]);
    }
}
