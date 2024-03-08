#include "stdio.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc,char **argv)
{
    char data[2] = {0};
    int num;
    int fd = open("/dev/tem_device",O_RDWR);
    while (1)
    {
    num = read(fd,data,2);
    if(num != 0)
    {
        fprintf(stderr,"read is error num is %d",num);
    }
    printf("tem is %d &&&&& humi is %d\n ",data[0],data[1]);
    sleep(1);
    }  
    return 0;
}