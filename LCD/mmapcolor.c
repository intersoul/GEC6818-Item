#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **agrv)
{
    int fd_lcd;
    int j = 0;
    int *addr = NULL;
    // int color = 0x00ff00ff;
    int color[6] = {0x00ff0000,
                    0x0000ff00,
                    0x000000ff,
                    0x00ffff00,
                    0x00ff00ff,
                    0x0000ffff};

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

    while (1)
    {
        // 给指针指向的这段空间赋值
        int x = 0; // x是横轴
        int y = 0; // y是纵轴
        for (y = 0; y < 480; y++)
        {
            for (x = 0; x < 800; x++)
                // 指针偏移是指向的下一个单位
                // 先指针偏移然后解引用
                *(addr + y * 800 + x) = color[j]; // 颜色
        }
        j++;
        if (j >= 6)
            j = 0;

        sleep(1); // 睡眠1秒
    }

    // 取消内存映射（munmap）
    munmap(addr, 800 * 480 * 4);

    close(fd_lcd);

    return 0;
}