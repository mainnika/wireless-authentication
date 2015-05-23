#pragma once
#include <cstdlib>

struct free_delete
{
    void operator()(void* x) { free(x); }
};