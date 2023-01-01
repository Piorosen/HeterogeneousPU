#ifndef MAIN_COMPOSE_MANAGER_H
#define MAIN_COMPOSE_MANAGER_H

#include <vector>
#include <memory>
#include <string>

#include <compose/iengine.h>

namespace compose { 


enum engine { 
    myriad,
    neon,
    rknn,
    mali,
    coral
};

std::string engine_to_string(engine e);

class manager {
public:
    static manager* instance() { 
        static manager inst;
        return &inst;
    }

    std::vector<engine> engine_list() const;

    std::vector<std::shared_ptr<iengine>> inference_engine();

    std::shared_ptr<iengine> create(engine) const;
};

}



#endif // MAIN_COMPOSE_MANAGER_H
