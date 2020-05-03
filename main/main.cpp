// OpenGLScratch.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "rslib.h"
#include "basiclighting.h"
#include "simple.h"
#include "modelloading.h"

int main(int argc, char**argv)
{
    RSLib::instance()->initConfig(argc, argv);
    BasicLighting light;
    Simple s;
    ModelLoading m;
    m.run();
    return 0;
}
