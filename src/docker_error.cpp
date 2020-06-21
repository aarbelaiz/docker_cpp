#include "docker_cpp/docker_error.h"

namespace docker_cpp
{
    DockerError::DockerError(dockErr code, const std::string& msg, const int codeAPI)
    {
        this->errorCode = code;
        this->msg = msg;
        this->apiErrorCode = codeAPI;
    }

    bool DockerError::isOk()
    {
        return errorCode == DOCKER_OK || errorCode == DOCKER_INFO;
    }

    bool DockerError::isError()
    {
        return errorCode == DOCKER_ERROR;
    }

    DockerError DockerError::D_OK()
    {
        return DockerError(DOCKER_OK, "", 200);
    }

    DockerError DockerError::D_INFO(const std::string& msg, int codeAPI)
    {
        return DockerError(DOCKER_INFO, msg, codeAPI);
    }

    DockerError DockerError::D_ERROR(const std::string& msg, int codeAPI)
    {
        return DockerError(DOCKER_ERROR, msg, codeAPI);
    }

    std::ostream& operator<<(std::ostream& os, const DockerError& err)
    {
        os << "(" << err.errorCode << ")[" << err.apiErrorCode << "] : " << err.msg;
        return os;
    }
} // namespace docker_cpp
