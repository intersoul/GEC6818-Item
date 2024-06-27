#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/mman.h>

// 显示图片的函数
int show_pic_any(char *filename, int *p, int x_offset, int y_offset)
{
    // 打开图片

    // 打开图片
    int bmp_fd = open("./zjj.bmp", O_RDWR);
    if (bmp_fd < 0)
    {
        printf("图片打开失败\n");
        return -1;
    }
    // 读取图片前54字节的信息头
    char head[54];
    read(bmp_fd, head, 54);

    // 计算图片的宽度与高度,18-21字节表示宽度，22-25字节表示图片的高度
    // 18字节是最低为，21字节是最高位
    int wide = head[18] | head[19] << 8 | head[20] << 16 | head[21] << 24; // 宽度
    int high = head[22] | head[23] << 8 | head[24] << 16 | head[25] << 24; // 高度

    printf("wide = %d high = %d\n", wide, high);

    // 读取剩下的颜色数据
    char buf[wide * high * 3];
    int num = (wide * 3) % 4; // 判断图片的宽度是不是4的整数倍
    if (num == 0)             // 是4的倍数, 全部读取
    {
        read(bmp_fd, buf, sizeof(buf));
    }
    else // 不是4的倍数，一行一行的去读
    {
        for (int i = 0; i < high; i++)
        {
            read(bmp_fd, buf + i * wide * 3, wide * 3); // 读取一行
            lseek(bmp_fd, 4 - num, SEEK_CUR);           // 便宜掉系统自动补齐的数据
        }
    }
    // 显示图片
    int i = 0, value = 0;
    for (int y = 0; y < high; y++) // 外层循环来控制行数
    {
        for (int x = 0; x < wide; x++) // 内层循环来控制列数
        {
            char b = buf[i++];
            char g = buf[i++];
            char r = buf[i++];
            value = b | g << 8 | r << 16;
            // *(p+(800*y+x)) = value;
            // 图片的偏移量，纵轴偏移量加上图片本身的高度不能大于480
            // 横轴偏移量加上图片本身的宽度不能大于800，否则有可能会出现段错误
            *(p + (800 * (high - 1 - y + y_offset) + x + x_offset)) = value;
        }
    }
    // 关闭设备
    close(bmp_fd);
}

int main(int argc, char const *argv[])
{
    // 第一步：打开显示屏
    int lcd_fd = open("/dev/fb0", O_RDWR);
    if (lcd_fd < 0)
    {
        printf("显示屏打开失败\n");
        return -1;
    }
    // 进行内存映射
    int *p = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd_fd, 0);
    if (p == NULL)
    {
        printf("内存映射失败\n");
        return -1;
    }

    // 第二步：显示图片
    show_pic_any("./zjja.bmp", p, 0, 0);

    // 第三步：关闭显示屏
    munmap(p, 800 * 480 * 4); // 解除内存映射

    close(lcd_fd);

    return 0;
}
