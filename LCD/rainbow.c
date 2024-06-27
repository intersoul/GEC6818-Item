#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **agrv)
{
    int width = 68; // 宽度
    int temp = 0;   // 临时变量
    int fd_lcd;
    int j = 0;
    int *addr = NULL;
    // int color = 0x00ff00ff;
    int color[7] = {0x00ff0000,
                    0x0000ff00,
                    0x000000ff,
                    0x00ffff00,
                    0x00ff00ff,
                    0x0000ffff,
                    0x00f00f0f};

    // 1、打开lcd（open）
    fd_lcd = open("/dev/fb0", O_RDWR);
    if (fd_lcd == -1)
    {
        perror("open lcd fail");
        return 0;
    }

    // 2、内存映射（mmap）
    addr = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd_lcd, 0);
    if (addr == NULL)
    {
        perror("mmap fail");
        return -1;
    }

    // 给指针指向的这段空间赋值
    int x = 0; // x是横轴
    int y = 0; // y是纵轴
    for (int i = 0; i < 7; i++)
    {
        for (y = temp; y < width; y++)
        {
            for (x = 0; x < 800; x++)
                // 指针偏移是指向的下一个单位
                // 先指针偏移然后解引用
                *(addr + y * 800 + x) = color[i]; // 颜色
        }
        printf("width = %d\n", width);
        printf("temp = %d\n", temp);
        temp = width;
        width = width + 68; // 宽度增加68
    }

    // 取消内存映射（munmap）
    munmap(addr, 800 * 480 * 4);

    close(fd_lcd);

    return 0;
}