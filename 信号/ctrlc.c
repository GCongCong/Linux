# include<stdio.h>
# include<stdlib.h>
# include<assert.h>
# include<string.h>
# include<signal.h>

void fun(int sign)
{
    printf("Hello Linux! sign=%d\n",sign);
}

int main()
{
    signal(SIGINT,fun);
    while(1)
    {
        sleep(5);
        printf("Ctrl c is trapeed\n");
    }
    exit(0);
}
