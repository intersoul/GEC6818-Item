#include <stdio.h>

// argc:表示参数的个数，注意可执行文件本身也要占据一个参数
// argv:指针数组, 代表数组的元素是指针，指向命令终端的参数，并且主函数的参数是以字符串的形式传递
int main(int argc, char const *argv[]) // ./a.out 111 222
{
    printf("argc = %d\n", argc);       // argc = 3
    printf("argv[0] = %s\n", argv[0]); // ./a.out
    printf("argv[1] = %s\n", argv[1]); // 111
    printf("argv[2] = %s\n", argv[2]); // 222
    return 0;
}