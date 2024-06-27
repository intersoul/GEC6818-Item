#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <math.h>

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
    munmap(plcd, 800 * 480 * 4);
    close(lcd_fd);
}

void point(int x, int y, unsigned int color)
{
    if (x >= 0 && x < 800 && y >= 0 && y < 480)
    {
        *(plcd + y * 800 + x) = color;
    }
}

void display_sql(int w, int h, int x0, int y0, int color)
{
    int x, y;
    for (y = 0; y < h; y++)
    {
        for (x = 0; x < w; x++)
        {
            point(x + x0, y + y0, color);
        }
    }
}

void fill_circle(int xc, int yc, int r, unsigned int color)
{
    int x = 0, y = r;
    int d = 3 - 2 * r;

    while (x <= y)
    {
        // 画圆弧上的水平线
        for (int i = xc - x; i <= xc + x; i++)
        {
            point(i, yc - y, color);
            point(i, yc + y, color);
        }

        // 画圆弧上的垂直线
        for (int i = xc - y; i <= xc + y; i++)
        {
            point(i, yc - x, color);
            point(i, yc + x, color);
        }

        if (d < 0)
        {
            d = d + 4 * x + 6;
        }
        else
        {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

int main(int argc, char *argv[])
{
    lcdinit();

    display_sql(200, 200, 100, 100, 0xff0000);
    display_sql(200, 200, 400, 100, 0xff);

    // 画一个填充的黄色圆形
    fill_circle(300, 240, 50, 0xffff00);

    lcd_destory();
    return 0;
}
