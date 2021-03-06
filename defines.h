#ifndef DEFINES_H
#define DEFINES_H

#include <stdio.h>

#define EXIT_ERROR(error) \
    fprintf(stderr, error); \
    exit(1);

#define EXIT_ERRORF(errorf, ...) \
    fprintf(stderr, errorf, ##__VA_ARGS__); \
    exit(1);

#endif //DEFINES_H
