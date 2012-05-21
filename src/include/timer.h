#ifndef TIMER_H_
#define TIMER_H_

/* timing functions */
#ifdef __unix__
#include <sys/time.h>
#elif  _WIN32
#include <Windows.h>
__int64 _freq;
#endif

void init_timer(){
#ifdef _WIN32
	QueryPerformanceFrequency((LARGE_INTEGER *)&_freq);  /* needs Windows.h header */
#endif
}

void timer(double *et /* microseconds */ ){
#ifdef __unix__
	struct timeval t;
    gettimeofday( &t, (struct timezone *)0 );
    *et = t.tv_sec*1.0e3 + t.tv_usec*1.0e-3;
#elif _WIN32
    __int64 t;
    QueryPerformanceCounter((LARGE_INTEGER *)&t);   /* needs Windows.h header */
    *et = (double)t * 1000 / (double)_freq;
#endif
}

#endif // TIMER_H_
