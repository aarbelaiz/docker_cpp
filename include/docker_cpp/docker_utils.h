#ifndef _DOCKER_UTILS_H
#define _DOCKER_UTILS_H

#include "export.h"

#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <type_traits>
#include <iostream>
namespace docker_cpp {
    std::string DOCKER_CPP_API _map2json(const std::map<std::string, std::string> &in);
}

#endif // _DOCKER_UTILS_H
