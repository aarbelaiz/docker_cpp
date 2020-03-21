#ifndef _DOCKER_ERROR_H
#define _DOCKER_ERROR_H

#include "docker_types.h"
#include <string>
#include <iostream>

namespace docker_cpp {

    class DOCKER_CPP_API DockerError {
        public:
            DockerError(dockErr code, const std::string& msg, int codeAPI);
            bool isValid();
            friend std::ostream& operator<<(std::ostream& os, const DockerError& err);

            std::string msg;
            dockErr errorCode;
            int apiErrorCode;
    };
}

#endif //_DOCKER_ERROR_H
