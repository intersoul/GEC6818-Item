#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;
    char buf[100];
    int offset;
    fd = open("2.txt", O_RDONLY);
    if (fd < 0)
    {
        printf("error");
        return -1;
    }
    offset = lseek(fd, 0, SEEK_SET);
    read(fd, buf, 100);
    printf("rd_buf = %s\r\n", buf); // 打印数据
    offset = lseek(fd, 0, SEEK_END);

    printf("file size is %d\n", offset);
    close(fd);
    return 0;
}
