# include<stdio.h>
# include<stdlib.h>
# include<unistd.h>
# include<assert.h>
# include<sys/sem.h>


//保存信号量值的联合体
union semval
{
    int val;
};
//创建/获得信号量集合
int CreateSem(int key,int init_val[],int len);
//减1操作
void SemP(int semid,int index);
//加1操作
void SemV(int semid,int index);
//删除信号量集合
void DeleteSem(int semid);
