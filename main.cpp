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
    auto eft = scheduler::instance()->first_come_first_served();
    // vector<string> models { "resnet101"  };
    vector<string> models { "mobilenet" };
    eft->init(models, 32);
    printf("hahaha\n\n");
    eft->sequence(models);

    return 0;
}