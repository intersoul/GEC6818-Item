#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    // 打开文件
    int fd = open("test1.txt", O_RDWR | O_CREAT); // 以可读可写的方式打开
    if (fd < 0)
    {
        printf("打开文件失败\n");
        return -1;
    }

    // 写入数据
    char buf[128] = "hello world,我是SOUL";
    ssize_t wr = write(fd, buf, strlen(buf)); // sizeof

    fd = open("test1.txt", O_RDWR | O_CREAT); // 以可读可写的方式打开

    // 读取文件内容
    char rw_buf[128];
    ssize_t red = read(fd, rw_buf, strlen(buf));

    printf("rw_buf = %s\n", rw_buf);

    // 打开文件
    int fd2 = open("test2.txt", O_RDWR | O_CREAT); // 以可读可写的方式打开
    if (fd2 < 0)
    {
        printf("打开文件失败\n");
        return -1;
    }
    int i = 0;
    int j = 0;
    char wr_buf[128];
    // 写入数据

    for (i = 0; i < strlen(rw_buf); i++)
    {
        wr_buf[i] = rw_buf[i];
    }
    // printf("wr_buf = %s\n", wr_buf);

    wr = write(fd2, wr_buf, strlen(wr_buf)); // sizeof

    fd2 = open("test2.txt", O_RDWR | O_CREAT); // 以可读可写的方式打开

    red = read(fd2, rw_buf, strlen(buf));

    printf("rw_buf2 = %s\n", rw_buf);
    printf("red = %ld\n", red);

    close(fd);
    close(fd2);

    return 0;
}