# include<stdio.h>
# include<stdlib.h>
# include<unistd.h>
# include<fcntl.h>
# include<string.h>
# include<assert.h>

int main()
{
    int fd=open("./a.txt",O_RDONLY);
    assert(fd!=-1);

    pid_t pid=fork();
    assert(pid!=-1);
    if(pid==0)
    {
        char child=0;
        while(1)
        {
            int n=read(fd,&child,1);
            if(n<=0)
            {
                printf("child read over\n");
                break;
            }
            else
            {
               printf("child:%c\n",child);
               sleep(1); 
            }
        }
    }
    else
    {
        char father=0;
        while(1)
        {
            int n=read(fd,&father,1);
            if(n<=0)
            {
                printf("father read over\n");
                break;
            }
            else
            {
                printf("father:%c\n",father);
                sleep(1);
            }
       }
    }
    
    close(fd);
    exit(0);
}
