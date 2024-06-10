#ifndef TIMEIT_H
#define TIMEIT_H

#include <time.h>
#include <stdio.h>

#define TIME_IT(code) \
    do { \
        clock_t start = clock(); \
        code; \
        clock_t end = clock(); \
        double time_spent = ((double)(end - start)) / CLOCKS_PER_SEC; \
        printf("Time spent: %f\n", time_spent); \
    } while (0)
#endif