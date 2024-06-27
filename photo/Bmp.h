#ifndef __BMP_H_
#define __BMP_H_

//头文件
#include "lcd.h"

//-------函数声明-----------

//循环播放图片
void bmp_player();

//显示某张图片
int show_bmp(char * filename,int x0,int y0);


#endif