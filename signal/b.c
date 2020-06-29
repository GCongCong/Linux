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
    int semid=CreateSem(1234,&val,1);//b后运行，直接获取到信号量
    assert(semid!=-1);

    int count=0;//使用打印机次数

    while(1)
    {
        SemP(semid,0);//P，占有打印机
        printf("b\n");
        sleep(5);
        printf("b\n");
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
