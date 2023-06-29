#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <gmpxx.h>
#include <vector>

#include "Chrono.hpp"

#define MILLERRABINPARAM 20
#define MAXCHARSIZE 1000
#define BASE 10

struct arg_struct {
    std::string inf;
    std::string sup;
};

void* findFirstNumbers(void* arguments);
int readLine(char* line);