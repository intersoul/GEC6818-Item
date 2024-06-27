#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[])
{
    // 1、打开lcd
    int j = 0;
    int fd_lcd, ret; // --->  lcd也是一个文件，需要使用一个文件描述符
    fd_lcd = open("/dev/fb0", O_RDWR);
    if (fd_lcd == -1)
    {
        perror("open lcd fail");
        return -1;
    }

    while (1)
    {
        int buf[800 * 480] = {0};
        int color[6] = {0x00ff0000,
                        0x0000ff00,
                        0x000000ff,
                        0x00ffff00,
                        0x00ff00ff,
                        0x0000ffff};
        // int color[6] = {0x00ff00};
        for (j = 0; j < 6; j++)
        {
            for (int i = 0; i < 800 * 480; i++)
            {
                buf[i] = color[j];
            }
            printf("j=%d\n", j);
        }

        // j++;

        // if (j >= 6)
        // {
        //     j = 0;
        //     /* code */
        // }

        lseek(fd_lcd, 0, SEEK_SET); // 移动到lcd的起始位置

        sleep(1); // 睡眠1秒
        // 刷新lcd
        write(fd_lcd, buf, sizeof(buf));
    }

    // int buf[800 * 480] = {0};
    // int x = 0; // x表示lcd的横轴
    // int y = 0; // y表示lcd的纵轴
    // for (y = 0; y < 480; y++)
    // {
    //     for (x = 0; x < 800; x++)
    //     {
    //         // y=0 x=0 第0行第0个像素点
    //         // y=1 x=1 第1行的第1个像素点(4个字节)
    //         // buf[y * 800 + x] = color[0];
    //     }
    // }

    // 3、将buf写到lcd中
    // write(fd_lcd, buf, sizeof(buf));

    // 4、关闭lcd
    close(fd_lcd);

    return 0;
}