#ifndef __LCD_H_
#define __LCD_H_

//------头文件声明-------
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>

//------函数声明-------

//屏幕打开
int lcd_init();
// 屏幕测试
void lcd_test();
// 屏幕关闭
void lcd_close();
// 屏幕显示颜色
void display_point(int x,int y,int color);  //x为高，y为宽


#endif