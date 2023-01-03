#include <scheduler/scheduler.h>

#include <iostream>
#include <compose/manager.h>

#include <chrono>
#include <string>
#include <vector>
#include <random>

using namespace std;
using namespace std::chrono;

int main(int argc, char **argv)
{
    auto eft = scheduler::instance()->max_min();
    // vector<string> models { "resnet50", "resnet1001", "mobilenet"  };
    vector<string> models { "resnet101", "mobilenet", "resnet50",   };
    eft->init(models, 0);
    printf("hahaha\n\n");
    eft->sequence(models);

    return 0;
}