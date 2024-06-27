// #include <stdio.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <unistd.h>

// int main(int argc, char *argv[])
// {
//     int flag;
//     flag = open("test1.txt", O_RDWR | O_CREAT, S_IRWXU);
//     if (flag == -1)
//     {
//         perror("open");
//         return 1;
//     }
//     printf("%d\n", flag);
//     printf("open success\n");
//     close(flag);
//     return 0;
// }

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
    int fd = open("1.txt", O_RDWR | O_CREAT | O_TRUNC, 0777);
    if (fd < 0)
    {
        printf("打开文件失败\n");
        return -1;
    }
    printf("fd = %d\n", fd);

    return 0;
}