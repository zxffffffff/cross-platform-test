#pragma once

#include <iostream>
#include <string.h>
#include <cassert>
#include <thread>

class Common
{
private:
    Common();
    ~Common();

public:
    static Common& Singleton()
    {
        static Common ins;
        return ins;
    }
};
