#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>

int lcd_fd = -1; // 全局的lcd描述符

unsigned int *plcd = NULL;

void lcdinit()
{
    lcd_fd = open("/dev/fb0", O_RDWR);
    if (-1 == lcd_fd)
    {
        perror("open fb0 error");
        exit(1);
    }
    plcd = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd_fd, 0);
    if (plcd == MAP_FAILED)
    {
        perror("mmap error");
        return;
    }
}
void lcd_destory()
{
    munmap(plcd, 800 * 400 * 4);
    close(lcd_fd);
}

/*画点函数
 * 传入参数：
 * (x,y)需要画点的坐标
 *   color:(x,y)点的颜色
 */
void point(int x, int y, unsigned int color)
{
    if (x > 0 && x < 800 && y >= 0 && y < 480)
    {
        *(plcd + y * 800 + x) = color; // 给这个点赋值color
    }
}

/*函数：画一个方块
 * 传入参数：
 * w: 正方形的宽
 * h: 正方形的高
 * x0: 需要画在x0的位置
 * y0: 需要画在y0的位置
 * color : 需要画的颜色
 * 返回值：NULL
 */
void display_sql(int w, int h, int x0, int y0, int color)
{
    int x, y;
    for (y = 0; y < h; y++)
    {
        for (x = 0; x < w; x++)
        {
            point(x + x0, y + y0, color); // 在x0,y0开始的位置开始画点
        }
    }
}
int main()
{
    int r1, r2;
    lcdinit();
    // 操作屏幕
    display_sql(200, 200, 100, 100, 0xff0000);
    display_sql(200, 200, 400, 100, 0xff);
    lcd_destory();
    return 0;
}
