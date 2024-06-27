#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *task1(void *arg)
{
    while (1)
    {
        printf("task1 running\n");
        sleep(1);
    }
}

void *task2(void *arg)
{
    while (1)
    {
        printf("task2 running\n");
        sleep(1);
    }
}

int main(int argc, char *argv[])
{
    pthread_t t1, t2;
    pthread_create(&t1, NULL, task1, NULL);
    pthread_create(&t2, NULL, task2, NULL);

    int n;
    while (1)
    {
        scanf("%d", &n);
        if (n == 0)
        {
            pthread_cancel(t1);
        }

        else if (n == 1)
        {
            pthread_cancel(t2);
        }
    }
    return 0;
}