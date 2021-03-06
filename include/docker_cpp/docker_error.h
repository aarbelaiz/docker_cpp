#ifndef _DOCKER_ERROR_H
#define _DOCKER_ERROR_H

#include "docker_cpp/docker_types.h"

#include <string>
#include <iostream>

namespace docker_cpp {
    class DOCKER_CPP_API DockerError {
        public:
            DockerError(dockErr code, const std::string& msg, int codeAPI);
            bool isOk();
            bool isError();
            friend std::ostream& operator<<(std::ostream& os, const DockerError& err);

            static DockerError D_OK();
            static DockerError D_INFO(const std::string& msg, int codeAPI);
            static DockerError D_ERROR(const std::string& msg, int codeAPI);

            std::string msg;
            dockErr errorCode;
            int apiErrorCode;
    };
}

#endif //_DOCKER_ERROR_H
