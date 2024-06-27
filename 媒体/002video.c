#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    system("mplayer -x 800 -y 480 -zoom ruoshui.mp4 * < /dev/null > /dev/null 2>1&\n");
    return 0;
}