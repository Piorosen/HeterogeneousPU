#ifndef MAIN_ARGUMENT_OPTION_H
#define MAIN_ARGUMENT_OPTION_H

#include <string>

struct option
{
    std::string model_path;
    std::string image_path;
    
    std::string data_layout;

    int channel;
    int width;
    int height;
    int batch;
};

#endif // MAIN_ARGUMENT_OPTION_H
