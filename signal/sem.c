# include<stdio.h>
# include<stdlib.h>
# include<unistd.h>
# include<assert.h>
# include<sys/sem.h>
# include<string.h>
# include "./sem.h"

//创建/获得信号量集合
int CreateSem(int key,int init_val[],int len)
{
    //
    int semid=semget((key_t)key,0,0664);
    if(semid!=-1)
    {
        return semid;
    }
    //
    semid=semget((key_t)key,len,IPC_CREAT|0664);
    if(semid==-1)
    {
        perror("Create Sem Error\n");
        return -1;
    }
    //
    int i=0;
    for(;i<len;i++)
    {
        union semval data;
        data.val=init_val[i];
        if(semctl(semid,i,SETVAL,data)==-1)
        {
            perror("Init Sem Value Fail\n");
            return -1;
        }
    }
    return semid;
}
//减1操作
void SemP(int semid,int index)
{
    struct sembuf buf;
    buf.sem_num=index;
    buf.sem_op=-1;//
    buf.sem_flg=SEM_UNDO;

    if(semop(semid,&buf,1)==-1)
    {
        perror("Sem P Error\n");
    }
}
//加1操作
void SemV(int semid,int index)
{
    struct sembuf buf;
    buf.sem_num=index;
    buf.sem_op=1;
    buf.sem_flg=SEM_UNDO;
    if(semop(semid,&buf,1)==-1)
    {
        perror("Sem V Error\n");
    }
}
//删除信号量集合
void DeleteSem(int semid)
{
    if(semctl(semid,0,IPC_RMID)==-1)
    {
        perror("Delete Error\n");
    }

}
