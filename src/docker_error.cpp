#include "docker_cpp/docker_error.h"

namespace docker_cpp
{
    DockerError::DockerError(dockErr code, const std::string& msg, const int codeAPI)
    {
        this->errorCode = code;
        this->msg = msg;
        this->apiErrorCode = codeAPI;
    }

    bool DockerError::isValid()
    {
        return errorCode == DOCKER_OK;
    }

    std::ostream& operator<<(std::ostream& os, const DockerError& err)
    {
        os << "(" << err.errorCode << ")[" << err.apiErrorCode << "] : " << err.msg;
        return os;
    }
} // namespace docker_cpp
