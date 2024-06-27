#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

int main(int argc, char *argv[])
{
    int ts_fd;
    struct input_event ts_buf;
    ts_fd = open("/dev/input/event0", O_RDONLY);
    if (ts_fd == -1)
    {
        printf("open ts failed!\n");
        return -1;
    }
    int x, y;
    while (1)
    {
        read(ts_fd, &ts_buf, sizeof(ts_buf));
        if (ts_buf.type == EV_ABS && ts_buf.code == ABS_X) // 判断是否发生X轴绝对位移事件
        {
            x = ts_buf.value;
        }
        if (ts_buf.type == EV_ABS && ts_buf.code == ABS_Y) // 判断是否发生Y轴绝对位移事件
        {
            y = ts_buf.value;
        }
        if (ts_buf.type == EV_KEY && ts_buf.code == BTN_TOUCH && ts_buf.value == 0) // 松手检测
        {
            printf("(x, y) = (%d, %d)\n", x, y);
        }
    }
    close(ts_fd);
    return 0;
}