#ifndef __TIMER_H__
#define __TIMER_H__

/* timing functions */
#ifdef _WIN32
#include <Windows.h>
__int64 _freq;
void init_timer(){
    QueryPerformanceFrequency((LARGE_INTEGER *)&_freq);  /* needs Windows.h header */
}
void timer(double *et /* microseconds */ ){
    __int64 t;
    QueryPerformanceCounter((LARGE_INTEGER *)&t);   /* needs Windows.h header */
    *et = (double)t * 1000 / (double)_freq;
}
#elif __unix__
#include <sys/time.h>
void timer(double *et /* microseconds */ ){
    struct timeval t;
    gettimeofday( &t, (struct timezone *)0 );
    *et = t.tv_sec*1.0e3 + t.tv_usec*1.0e-3;
}
#endif
#endif
