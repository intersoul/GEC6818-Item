#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/mman.h>
#include <linux/input.h>

// 显示图片的函数
int show_pic_any(char *filename, int *p, int x_offset, int y_offset)
{
    printf("x1 = %d, y1 = %d\n", x_offset, y_offset);
    // 打开图片
    int bmp_fd = open(filename, O_RDWR);
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

int get_xy(int touch_fd, int *x, int *y)
{
    // 读取触摸屏信息
    struct input_event info;
    int temp_x, temp_y, x1, x2, y1, y2, var_x, var_y;
    while (1)
    {
        read(touch_fd, &info, sizeof(info)); // 阻塞，等待输入设备发生变化
        if (info.type == EV_ABS)             // 判断是不是绝对事件
        {
            if (info.code == ABS_X) // 是不是x轴
            {
                temp_x = info.value;
                // printf("x = %d\n", info.value); // 蓝色屏幕
                // printf("x = %d\n", info.value*800/1024);    // 黑色屏幕
            }
            if (info.code == ABS_Y) // 是不是x轴
            {
                temp_y = info.value;
                // printf("y = %d\n", info.value); // 蓝色屏幕
                // printf("y = %d\n", info.value*480/600); //黑色屏幕
            }
        }
        // 判断手指是按下还是松开，按下时获取第一组坐标，松开的时获取第二组坐标
        if (info.type == EV_KEY && info.code == BTN_TOUCH)
        {
            if (info.value == 1) // 按下，获取第一组坐标
            {
                x1 = temp_x;
                y1 = temp_y;
            }
            else if (info.value == 0) // 松开，获取第二组坐标
            {
                x2 = temp_x;
                y2 = temp_y;
                break;
            }
        }
    }
    // 计算第一组坐标与最后一组坐标的差值
    var_x = x2 - x1;
    var_y = y2 - y1;

    // 判断划动的距离，如果距离大于60就设置为划动，否则就视为点击
    if ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) >= 60 * 60) // 划动
    {
        if (var_y < 0 && abs(var_y) > abs(var_x)) // abs取绝对值 , 上划
            return 1;
        else if (var_y > 0 && abs(var_y) > abs(var_x)) // 向下划动
            return 2;
        else if (var_x < 0 && abs(var_y) < abs(var_x)) // 左划
            return 3;
        else if (var_x > 0 && abs(var_y) < abs(var_x)) // 右划
            return 4;
    }
    else // 点击
    {
        *x = x2;
        *y = y2;
        return 0;
    }
}

int main(int argc, char const *argv[])
{
    // 打开触摸屏
    int touch_fd = open("/dev/input/event0", O_RDWR);
    if (touch_fd < 0)
    {
        printf("触摸屏打开失败\n");
        return -1;
    }

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

    int x, y;

    for (y = 0; y < 480; y++)
    {
        for (x = 0; x < 800; x++)
            *(p + y * 800 + x) = 0x0000ff00; // 颜色
    }

    // show_pic_any("./zjj.bmp", p, 50, 10);
    int photo = 0;

    while (1)
    {
        int position = get_xy(touch_fd, &x, &y);
        // 第二步：显示图片

        if (position == 1)
        {
            // show_pic_any("./zjj.bmp", p, x, y);
            printf("向上划动\n");
        }
        else if (position == 2)
            printf("向下划动\n");
        else if (position == 3)
            printf("向左划动\n");
        else if (position == 4)
            printf("向右划动\n");
        else
        {

            printf("x = %d, y = %d\n", x, y);
            printf("photo = %d\n", photo);
            if (photo % 2 == 0)
            {
                for (y = 0; y < 480; y++)
                {
                    for (x = 0; x < 800; x++)
                        *(p + y * 800 + x) = 0x0000ff00; // 颜色
                }
                printf("显示图片1\n");
                show_pic_any("./2.bmp", p, 0, 0);
            }

            else
            {
                for (y = 0; y < 480; y++)
                {
                    for (x = 0; x < 800; x++)
                        *(p + y * 800 + x) = 0x0000ff00; // 颜色
                }
                printf("显示图片2\n");
                show_pic_any("./3.bmp", p, 0, 0);
            }

            photo++;
        }
    }

    // 关闭设备
    munmap(p, 800 * 480 * 4);
    close(touch_fd);
    return 0;
}
