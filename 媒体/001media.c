#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int n;
    while (1)
    {
        scanf("%d", &n);
        printf("You choose %d\n", n);
        switch (n)
        {
        case 1:
            printf("Playing music1.mp3\n");
            system("madplay ./music1.mp3 * < /dev/null > /dev/null 2>&1 &");
            break;
        case 2:
            printf("Playing music2.mp3\n");
            system("killall -STOP madplay");
            break;

        case 3:
            printf("Pause music\n");
            system("killall -CONT madplay");
            break;

        case 4:
            printf("Stop music\n");
            system("killall -KILL madplay");
            system("madplay ./music2.mp3 * < /dev/null > /dev/null 2>&1 &");
            break;

        case 5:
            printf("Exit program\n");
            system("killall -KILL madplay");
            return -1;
            break;
        }
    }
}