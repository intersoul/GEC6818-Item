#include "Bmp.h"
#include "lcd.h"

//显示图片
int show_bmp(char * filename,int x0,int y0)
{


    //1.打开bmp图片
    int fd = open(filename,O_RDWR);
    //判断读入图片是否错误
    if (-1 == fd)
    {
        printf("Open %s Fail!\n",filename);
        perror("--->");
        return -1;
    }

    //2.判断到底是不是一张bmp图片
	unsigned char buf[4];
	read(fd,buf,2);
	if(buf[0]!= 0x42 || buf[1]!= 0x4d)//若果不是B M 的ASCII码
	{
		printf("NOT BMP\n");
		goto ERROR_END;
	}

    //3.读取数据
    
    int width,height;
    short depth;
	lseek(fd,0x12,SEEK_SET);
	read(fd,buf,4);
	
	width=(buf[3]<<24)|
		  (buf[2]<<16)|
		  (buf[1]<<8)|
		  (buf[0]);

	lseek(fd,0x16,SEEK_SET);
	read(fd,buf,4);
	
	height=(buf[3]<<24)|
		  (buf[2]<<16)|
		  (buf[1]<<8)|
		  (buf[0]);

	lseek(fd,0x1c,SEEK_SET);
	read(fd,buf,2);
	
	depth=(buf[1]<<8)|
		  (buf[0]);

	//只支持色深为24和32的
	if(!(depth == 24 || depth == 32))
	{
		printf("NOT Support!\n");
		goto ERROR_END;
	}
	printf("%s:%d*%d depth=%d\n",filename,width,height,depth);

	//4.获取像素数组
	int line_valid_bytes = abs(width)*depth/8;//一行有效字节数
	int line_bytes;//一行总字节数=有效字节数+赖子数 
	int laizi = 0;
	
	if(line_valid_bytes%4)
	{
		laizi = 4-line_valid_bytes%4;
	}
	
	line_bytes = line_valid_bytes + laizi;
	
	int total_bytes = line_bytes*abs(height);//整个像素数组的大小


	//开辟一块动态内存
	unsigned char *piexl = (unsigned char *)malloc(total_bytes);    //用完后需要释放内存

	lseek(fd,54,SEEK_SET);
	read(fd,piexl,total_bytes);

    
	unsigned char a,r,g,b;
	int color;
	int i = 0;

	int x,y;
	for(y=0;y<abs(height);y++)
	{
		for(x=0;x<abs(width);x++)
		{
			//a r g b 0xargb 小端模式  b g r a
			b = piexl[i++];
			g = piexl[i++];
			r = piexl[i++];
			if(depth == 32)
			{
				a = piexl[i++];
			}
			else
			{
				a = 0;//不透明
			}
			color=(a<<24)|(r<<16)|(g<<8)|(b);

			//在屏幕对应的位置显示
			display_point(width>0?x0+x:x0+abs(width)-x-1, 
							height>0?y0+abs(height)-y-1:y0+y,
							color);
		}
		//每一行的末尾 有可能填充几个赖子
		i += laizi;
	}

    //释放内存
    free(piexl);
    //关闭显示
	close(fd);
    ERROR_END:
		close(fd);
		return -2;
}



//图片数组
// char * bmpname[] = {"1.bmp","2.bmp","3.bmp","4.bmp","5.bmp","6.bmp"};

//循环显示图片
void bmp_player(int i)
{
//     int i = 0;
//     while (1)
//     {
        //循环显示
        // show_bmp(bmpname[i],0,0);

    //     延时5s
        // sleep(5);
        // i++;
        // if (6 == i)
        // {
        //     i = 0;
        // }
        
    // }
    
}