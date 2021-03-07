#ifndef DEFINES_H
#define DEFINES_H

#include <stdio.h>

#define EXIT_ERROR(s) \
    fprintf(stderr, s); \
    exit(1);

#define EXIT_ERRORF(s, ...) \
    fprintf(stderr, s, ##__VA_ARGS__); \
    exit(1);

#define DEBUG

#ifdef DEBUG
#define DEBUGF(s, ...) printf(s,  ##__VA_ARGS__);
#else
#define DEBUGF(s, ...)
#endif

#endif //DEFINES_H
