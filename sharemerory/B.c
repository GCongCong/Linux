# include<stdio.h>
# include<stdlib.h>
# include<unistd.h>
# include<assert.h>
# include<sys/sem.h>
# include<sys/shm.h>
# include<string.h>
# include "./sem.h"
int main()
{
    int shmid=shmget((key_t)1234,128,IPC_CREAT|0664);//
    assert(shmid!=-1);

    char* ptr=(char*)shmat(shmid,NULL,0);
    assert(ptr!=(char*)-1);

    //创建两个信号量sem1控制B，sem2控制A
    int init_val[2]={0,1};
    int semid=CreateSem(1234,init_val,2);
    assert(semid!=-1);

    while(1)
    {
        SemP(semid,0);//对sem1进行P操作
        if(strncmp(ptr,"end",3)==0)
        {
            break;
        }
        printf("B process:%s\n",ptr);
        sleep(3);
        printf("B over\n");
        memset(ptr,0,128);
        SemV(semid,1);//对sem2进行V操作
    }
    shmdt(ptr);
    shmctl(shmid,IPC_RMID,NULL);
}

