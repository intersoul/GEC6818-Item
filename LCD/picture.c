#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/*在LCD上显示800*480像素点的bmp图片*/
int show_bmp(const char *bmp_name)
{
    int bmp_fd = open("bmp_name", O_RDWR);
    if (-1 == bmp_fd)
    {
        perror("open bmp failed");
        return -1;
    }
    // 2)读取图片像素点内容 -->跳过文件开头的54字节头信息 read
    char buf[800 * 480 * 3];        // 存放读取的bmp图片内容
    lseek(bmp_fd, 54, SEEK_SET);    // 跳过前54字节
    read(bmp_fd, buf, sizeof(buf)); // 读取像素点信息
    /// BGR --> ARGB//
    unsigned int bmp_buf[800 * 480]; // 800*480*4字节  转换之后的ARGB数据
    unsigned char A, R, G, B;        // 存放图片像素点的A R G B数据
    int i, j;
    unsigned int tmp;
    for (i = 0; i < 800 * 480; ++i)
    {
        A = 0x00;           // 透明度
        B = buf[3 * i];     // 蓝色
        G = buf[3 * i + 1]; // 绿色
        R = buf[3 * i + 2]; // 红色
        // A  R  G  B ==> ARGB   1234 = 1*1000 + 2*100 + 3*10 + 4
        // A << 24   R<< 16  G << 8  B
        bmp_buf[i] = A << 24 | R << 16 | G << 8 | B;
    }
    /// 图片数据实现翻转///
    for (i = 0; i < 480; ++i)
    {
        for (j = 0; j < 800; ++j)
        {
            // 把第i行，第j列的像素点跟 第479-i行，第j列的像素点进行交换
            tmp = bmp_buf[800 * i + j];
            bmp_buf[800 * i + j] = bmp_buf[800 * (479 - i) + j];
            bmp_buf[800 * (479 - i) + j] = tmp;
        }
    }
    //
    // 2,将读取的图片像素点数据写入到LCD
    // 1)打开LCD文件  --> /dev/fb0	open
    int lcd_fd = open("/dev/fb0", O_RDWR);
    if (-1 == lcd_fd)
    {
        perror("open lcd failed ");
        return -1;
    }
    // 2)把 RGB-> ARGB之后的像素点内容写入到LCD	  write
    write(lcd_fd, bmp_buf, sizeof(bmp_buf));

    // 3,关闭图片文件，关闭LCD文件	close
    close(bmp_fd);
    close(lcd_fd);

    return 0;
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        printf("请按照格式输入命令：%s <filename>\n", argv[0]);
        return -1;
    }

    show_bmp(argv[1]);
    return 0;
}
