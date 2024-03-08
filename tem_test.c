#include "stdio.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc,char **argv)
{
    open(argv,O_RDWR);


    return 0;
}