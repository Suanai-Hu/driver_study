#include <stdio.h>
#include <stsys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int fd1;
    int fd2;
    char buf1[1024] = "nihao /dev/test1";
    char buf2[1024] = "nihao /dev/test2";

    fd1 = open("/dev/test1", O_RDWR);
    if(fd1 < 0)
    {
        perror("open error!\n");
        return fd1;
    }
    write(fd1, buf1, sizeof(buf1));

    fd2 = open("/dev/test2", O_RDWR);
    if(fd2 < 0)
    {
        perror("open error!\n");
        return fd2;
    }
    write(fd2, buf2, sizeof(buf2));

    close(fd1);
    close(fd2);
    return 0;
}