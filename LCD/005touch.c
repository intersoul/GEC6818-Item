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

int main(int argc, char const *argv[])
{
    // 打开触摸屏
    int touch_fd = open("/dev/input/event0", O_RDWR);
    if (touch_fd < 0)
    {
        printf("触摸屏打开失败\n");
        return -1;
    }
    // 读取触摸屏信息
    struct input_event info;
    int x, y;
    while (1)
    {
        read(touch_fd, &info, sizeof(info)); // 阻塞，等待输入设备发生变化
        if (info.type == EV_ABS)             // 判断是不是绝对事件
        {
            if (info.code == ABS_X) // 是不是x轴
            {
                x = info.value;
                // printf("x = %d\n", info.value); // 蓝色屏幕
                // printf("x = %d\n", info.value*800/1024);    // 黑色屏幕
            }
            if (info.code == ABS_Y) // 是不是x轴
            {
                y = info.value;
                // printf("y = %d\n", info.value); // 蓝色屏幕
                // printf("y = %d\n", info.value*480/600); //黑色屏幕
            }
        }
        if (info.type == EV_KEY && info.code == BTN_TOUCH && info.value == 1) // 按下退出
            break;
    }
    printf("x = %d, y = %d\n", x, y);

    return 0;
}