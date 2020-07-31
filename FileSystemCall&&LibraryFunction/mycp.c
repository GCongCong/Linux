# include<stdio.h>
# include<string.h>
# include<assert.h>
# include<stdlib.h>

int main(int argc,char* argv[])
{
    char buff[128]={0};
    if(argc!=3)
    {
        printf("argv must have 3\n");
        exit(0);
    }
    FILE* file1=fopen(argv[1],"r");
    if(file1==NULL)
    {
        printf("first file error\n");
        exit(0);
    }
    else
    {
        while(1)
        {
            int n=fread(buff,sizeof(char),128,file1);
            if(n==0)
            {
                printf("----file1 fread over----\n");
                break;
            }
        }
    }
    FILE* file2=fopen(argv[2],"w");
    if(file2==NULL)
    {
        printf("second file error\n");
        exit(0);
    }
    int n=fwrite(buff,sizeof(char),strlen(buff),file2);
    printf("----file2 write over----\n");
    
    close(file1);
    fclose(file2);
    exit(0);
}
