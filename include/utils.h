#ifndef __UTILS_H
#define __UTILS_H

#include <stdio.h>

#define FINE 0
#define ERR (-1)

//#define DEBUG_ON
#ifdef DEBUG_ON
#define DEBUG(...) printf("[DEBUG] ");printf(__VA_ARGS__);
#else
#define DEBUG(...) 
#endif

#define ERROR(...) printf("[ERROR] ");printf(__VA_ARGS__);
#define INFO(...) printf("[INFO] ");printf(__VA_ARGS__);

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#endif
