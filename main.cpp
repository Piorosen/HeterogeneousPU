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
    auto eft = scheduler::instance()->fcfs_scheduler();
    vector<string> models { "resnet50", "mobilenet"  };
    // vector<string> models { "mobilenet", "resnet50", "resnet101" };
    eft->init(models, 32);
    printf("hahaha\n\n");
    eft->sequence(models);

    return 0;
}