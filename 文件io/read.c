#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int ret;
    int fd;

    char rd_buf[100];

    fd = open("test1.txt", O_RDWR); // 打开文件
    if (fd < 0)
    {
        printf("open file error...\r\n");
        return -1;
    }

    ret = read(fd, rd_buf, sizeof(rd_buf)); // 读取数据
    if (ret < 0)
    {
        printf("read file error...\r\n");
        return -1;
    }

    printf("rd_buf = %s\r\n", rd_buf); // 打印数据
    printf("ret = %d\r\n", ret);       // 打印读取的字节数

    close(fd); // 关闭文件

    return 0;
}
