#include <scheduler/scheduler.h>
#include <fstream>

#include <iostream>
#include <compose/manager.h>

#include <string>
#include <vector>

using namespace std;
using namespace std::chrono;

int main(int argc, char **argv)
{
    

    for (int i = 0; i < argc; i++) { 
        printf("%d : %s\n", i, argv[i]);
    }

    vector<string> models { "mobilenet", "resnet50", "resnet101" };
    ischeduler* sche;

    if (strcmp(argv[1], "affnity") == 0) { 
        sche = scheduler::instance()->affinity();
    }else if (strcmp(argv[1], "fcfs") == 0) { 
        sche = scheduler::instance()->first_come_first_served();

    }else if (strcmp(argv[1], "priority") == 0) { 
        sche = scheduler::instance()->priority();

    }else if (strcmp(argv[1], "sjf") == 0) { 
        sche = scheduler::instance()->short_job_first();
    }else { 
        printf("notfound : affnity, fcfs, priority, sjf\n");
        throw "not found";
    }
    sche->set_option(atoi(argv[2]), atoi(argv[3]));

    sche->init({ models[atoi(argv[4])], models[atoi(argv[5])], models[atoi(argv[6])] }, 0);

    sche->sequence({ models[atoi(argv[4])], models[atoi(argv[5])], models[atoi(argv[6])] });

    return 0;
}