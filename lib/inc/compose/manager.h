#ifndef MAIN_COMPOSE_MANAGER_H
#define MAIN_COMPOSE_MANAGER_H

#include <vector>
#include <memory>
#include <compose/iengine.h>

namespace compose { 
enum engine { 
    myriad,
    neon,
    rknn,
    mali,
    coral
};

class manager {
public:
    static manager* instance() { 
        static manager inst;
        return &inst;
    }

    std::vector<engine> engine_list() const;

    std::vector<std::shared_ptr<iengine>> inference_engine();

};

}



#endif // MAIN_COMPOSE_MANAGER_H
