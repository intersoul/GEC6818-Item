#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("请传递三个参数\n");
        return -1;
    }
    // 打开源文件, 以可读可写的权限去打开
    int src_fd = open(argv[1], O_RDWR);
    // 打开目标文件，以可读可写的方式打开，如果文件不存在则创建，存在则清空
    int dest_fd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0777);
    if (src_fd < 0 || dest_fd < 0)
    {
        printf("文件打开失败\n");
        return -1;
    }
    printf("开始复制\n");
    char buf[99999] = {0};
    while (1)
    {
        // 清空缓冲区
        memset(buf, 0, sizeof(buf));
        // 读取数据内容
        int red = read(src_fd, buf, sizeof(buf));
        if (red == 0) // 文件已经读取完毕
            break;
        // 写入到目标文件
        write(dest_fd, buf, red);
    }
    // 关闭文件
    close(src_fd);
    close(dest_fd);

    printf("复制完成\n");
    return 0;
}