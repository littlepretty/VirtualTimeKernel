#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <asm/unistd.h>
#include <pthread.h>

#define clock_getres(x,y) syscall(__NR_clock_getres, x,y)
#define clock_gettime(x,y) syscall(__NR_clock_gettime, x, y)
#define clock_settime(x,y) syscall(__NR_clock_settime, x, y)

void pr(int clock,const char *n)
{
        struct timespec tv = {1,2};
        struct timespec res = {3,4};
        int rc;


        rc=clock_getres(clock,&res);
        if (rc) {
                printf("getres return code on %s=%d errno=%d\n",n,rc,errno);
        }
        rc=clock_gettime(clock,&tv);
        if (rc) {
                printf("gettime return code on %s=%d errno=%d\n",n,rc, errno);
        } else
                printf("%25s=% 11d.%09d resolution=% 2d.%09d\n",n,tv.tv_sec,tv.tv_nsec,res.tv_sec,res.tv_nsec);
}

int y;

void kx(long long x) {
        y=x;
};

struct timespec zero;

pthread_t thread[10];

struct tinfo {
        int i;
        struct timespec ttime,ptime;
} tinf[10];

void *thread_function(void *x) {
        struct tinfo *t=x;
        int i;

        for(i=1;i< t->i;i++) kx(1000000000000LL/i);
        clock_gettime(CLOCK_THREAD_CPUTIME_ID,&t->ttime);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&t->ptime);
}

void do_gettime (struct timespec tv) {
        gettimeofday((struct timeval *)&tv);
        tv.tv_nsec = tv.tv_nsec*1000;
        printf("\n1st Clock status at the end of the timer tests:\n");
        printf("          Gettimeofday() =% 11d.%09d\n",tv.tv_sec,tv.tv_nsec);
        pr(CLOCK_REALTIME,"CLOCK_REALTIME");
        pr(CLOCK_MONOTONIC,"CLOCK_MONOTONIC");
}

int main(char argc, char *argv[])
{
        struct timespec tv;
        int i;

        /* Waste some time */
        /*printf("Single Thread Testing\n");*/

        /*for(i=1;i<10000000;i++) kx(1000000000000LL/i);*/
        /*pr(CLOCK_THREAD_CPUTIME_ID,"CLOCK_THREAD_CPUTIME_ID");*/
        /*pr(CLOCK_PROCESS_CPUTIME_ID,"CLOCK_PROCESS_CPUTIME_ID");*/
        /* Waste some more time in threads */
        /*printf("Multi Thread Testing\nStarting Thread:");*/
        /*clock_settime(CLOCK_PROCESS_CPUTIME_ID,&zero);*/
        /*for(i=0;i<10;i++) {*/
        /*tinf[i].i=i*1000000;*/
        /*if (pthread_create(&thread[i], NULL, thread_function, tinf+i))*/
        /*perror("thread");*/
        /*else*/
        /*printf(" %d",i);*/
        /*}*/
        /*printf("\n Joining Thread:");*/
        /*for(i=0;i<10;i++) if (pthread_join( thread[i], NULL)) perror("join"); else printf(" %d",i);*/
        /*printf("\n");*/
        /*for(i=0;i<10;i++) {*/
        /*printf("%d Cycles=%7d Thread=% 3d.%09dns Process=% 3d.%09dns\n",i,tinf[i].i,tinf[i].ttime.tv_sec,tinf[i].ttime.tv_nsec,tinf[i].ptime.tv_sec,tinf[i].ptime.tv_nsec);*/
        /*}*/
        int pid = getpid();
        printf("pid = %d\nPress any key + enter to continue", pid);
        char str1[10];
        while (fgets(str1, sizeof(str1), stdin) != NULL) {
                do_gettime(tv);
                printf("\nPress any key + enter to continue");
        }

        /*pr(CLOCK_PROCESS_CPUTIME_ID,"CLOCK_PROCESS_CPUTIME_ID");*/
        /*pr(CLOCK_THREAD_CPUTIME_ID,"CLOCK_THREAD_CPUTIME_ID");*/
        /*pr(10,"CLOCK_SGI_CYCLE");*/
        /*pr(-1,"PROCESS clock of 1 (init)");*/
        /*pr(-1 - 4*1024*1024,"THREAD clock of 1 (init)");*/
}
