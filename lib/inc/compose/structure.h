#ifndef MAIN_COMPOSE_DATA_STRUCTURE_H
#define MAIN_COMPOSE_DATA_STRUCTURE_H

namespace compose { 
enum data_layout { 
    nchw,
    nhwc
};

struct model_info {
    data_layout layout;

    int batch;
    int channel;
    int width;
    int height;
};
}


#endif // MAIN_COMPOSE_DATA_STRUCTURE_H
