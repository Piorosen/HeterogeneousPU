#ifndef MAIN_ARGUMENT_PARSER_H
#define MAIN_ARGUMENT_PARSER_H

#include <utility>

#include "option.h"

std::pair<bool, option> parser(int argc, char** argv);

#endif // MAIN_ARGUMENT_PARSER_H
