# include<stdio.h>
# include<stdlib.h>
# include<unistd.h>
# include<assert.h>
# include<sys/sem.h>
# include<string.h>
# include "./sem.h"

int main()
{
    int val=1;//信号量初始值
    int semid=CreateSem(1234,&val,1);//a先运行，创建信号集合IDkey为1234，值为1，1个信号量的信号集合
    assert(semid!=-1);

    int count=0;//使用打印机次数

    while(1)
    {
        SemP(semid,0);//P，占有打印机
        printf("a\n");
        sleep(5);
        printf("a\n");
        SemV(semid,0);//V，释放打印机
        sleep(2);
        count++;
        if(count==5)
        {
            break;
        }
    }
    exit(0);
}
