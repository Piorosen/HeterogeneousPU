#ifndef MAIN_COMPOSE_RKNNL_H
#define MAIN_COMPOSE_RKNNL_H
#include <vector>
#include <chrono>

#include <SimpleRKNN/simple_rknn.h>
using namespace std;
using namespace std::chrono;

std::vector<double> run_rknn() { 
    simple_rknn lib;
    lib.load_model("");
    return vector<double>();
}


#endif // MAIN_COMPOSE_RKNNL_H
