#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int ret;
    int fd;

    char wr_buf[100] = "hello world"; // 待写入数据

    fd = open("test1.txt", O_RDWR | O_TRUNC); // 打开文件
    if (fd < 0)
    {
        printf("open file error...\r\n");
        return -1;
    }

    // ret = write(fd, wr_buf, sizeof(wr_buf)); // 写入数据
    ret = write(fd, wr_buf, strlen(wr_buf)); // 写入数据
    if (ret < 0)
    {
        printf("write file error...\r\n");
        return -1;
    }

    lseek(fd, 0, SEEK_SET); // 移动文件指针到开头

    printf("rd_buf = %s\r\n", wr_buf); // 打印数据
    printf("ret = %d\r\n", ret);       // 打印读取的字节数

    close(fd); // 关闭文件

    return 0;
}
