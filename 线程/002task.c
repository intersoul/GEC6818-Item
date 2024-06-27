#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

struct parameter
{
    int num;
    char str;
    float fnum;
};

void *thread_func(void *arg)
{
    struct parameter *p = (struct parameter *)arg;
    while (1)
    {
        printf("num = %d, str = %c, fnum = %f\n", p->num, p->str, p->fnum);
        sleep(1);
    }

    // printf("num = %d, str = %c, fnum = %f\n", p->num, p->str, p->fnum);
    // return NULL;
}

int main()
{
    struct parameter p;
    p.num = 100;
    p.str = 'a';
    p.fnum = 3.14;

    pthread_t tid;
    // int arg = 100;
    pthread_create(&tid, NULL, thread_func, &p);
    while (1)
    {
    }

    return 0;
}