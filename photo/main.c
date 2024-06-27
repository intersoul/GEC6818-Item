#include "lcd.h"
#include "Bmp.h"
#include "touch.h"

char *bmpname[] = {"1.bmp", "2.bmp", "3.bmp", "4.bmp", "5.bmp", "6.bmp"};

// 主函数
int main(int argc, char const *argv[])
{
    int rs = 0;
    int i = 0;
    // 打开屏幕
    lcd_init();

    // 单独显示一张图片
    //  show_bmp("1.bmp",0,0);

    // 触摸
    while (1)
    {
        //  bmp_player(i);
        rs = GetDirection();
        printf("%d\n", rs);
        if (1 == rs || 3 == rs)
        {
            if (5 == i)
            {
                i = 0;
            }
            else
                ++i;
            show_bmp(bmpname[i], 0, 0);
            // bmp_player(i);
        }
        // show_bmp(bmpname[3],0,0);
        else if (2 == rs || 4 == rs)
        {
            if (0 == i)
            {
                i = 5;
            }
            else
                --i;
            // bmp_player(i);
            show_bmp(bmpname[i], 0, 0);
        }
    }

    // 循环显示
    //  bmp_player();

    // 关闭屏幕
    lcd_close();

    return 0;
}
