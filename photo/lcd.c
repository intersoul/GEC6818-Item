#include "lcd.h"

/*宏定义*/
#define BLUE 0x0000FF
#define LCD_PATH "/dev/fb0"

//全局变量
int fd = -1;
int *plcd = NULL;

//lcd初始化
int lcd_init()
{
    //1.打开屏幕文件
    fd = open(LCD_PATH,O_RDWR);//可读可写打开
    if(fd<0)
    {
        perror("open fail");
        return -1;
    }
    //2.映射 
    plcd = mmap(NULL, 
                800*480*4, 
                PROT_READ | PROT_WRITE, 
                MAP_SHARED,
                fd,
                0);
    if(plcd == MAP_FAILED)
    {
        perror("MAP fail");
        return -1;
    }
    return 0;
}


//LCD关闭
void lcd_close()
{
    munmap(plcd,800*480*4);
    close(fd);
}

//指定的点上显示指定的颜色
void display_point(int x,int y,int color)  //x为高，y为宽
{
    if(x >= 0 && x < 800 && y >= 0 && y < 480)
    {
        *(plcd + 800 * y + x) = color;
    }
}

//LCD测试
void lcd_test()
{
    lcd_init();

    int x,y;
    for(y=0;y<480;y++)//遍历每一行
    {
        for(x=0;x<800;x++)//遍历每一列
        {
            display_point(x,y,BLUE);
        }
    }
    // //写数据
    // write(fd,color,480*800*4);

    lcd_close(fd);
}
