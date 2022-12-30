#pragma once

// SJF (Shortest Job First)

// FCFS 알고리즘을 보완하기 위해 만들어졌다. 수행 시간이 가장 짧다고 판단되는 작업을 우선 수행한다. 평균 대기 시간이 짧으므로 짧은 작업에 유리하지만, 사용 시간이 긴 프로세스는 영원히 CPU를 할당 받지 못할 수 있다. (Starvation)

 

#include <scheduler/ischeduler.h>

class sjf : public ischeduler { 
public:
    virtual std::string get_name() const { return "sjf"; }
    virtual void init(const std::vector<std::string> models);
    virtual void deinit();
    virtual void sequence(std::vector<int> model_idx);
};
