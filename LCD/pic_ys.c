#include <stdio.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main(int argc, char const *argv[])
{

    int lcd_fd = open("/dev/fb0", O_RDWR);
    if (lcd_fd == -1)
    {
        perror("open lcd failed ");
        return -1;
    }
    // 2,读取图片像素点数据，并且进行解码
    int bmp_fd = open("./jiajiazi.bmp", O_RDWR);
    if (bmp_fd == -1)
    {
        perror("open bmp failed");
        return -1;
    }

    char buf[800 * 480 * 3];        // 存放读取的bmp图片内容
    lseek(bmp_fd, 54, SEEK_SET);    // 跳过前54字节
    read(bmp_fd, buf, sizeof(buf)); // 读取像素点信息

    // 3,将LCD映射到内存空间
    int *lcdmap = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd_fd, 0);
    if (lcdmap == MAP_FAILED)
    {
        perror("mmap failed");
        return -1;
    }

    int i;
    int value;
    for (int y = 0; y < 480; y++)
    {
        for (int x = 0; x < 800; x++)
        {
            char r = buf[i++];
            char g = buf[i++];
            char b = buf[i++];
            value = (r << 16) | (g << 8) | b;
            *(lcdmap + (800 * (479 - y) + x)) = value;
        }
    }

    // 5,解除LCD映射,关闭文件
    munmap(lcdmap, 800 * 480 * 4);
    close(lcd_fd);
    close(bmp_fd);
    return 0;
}
