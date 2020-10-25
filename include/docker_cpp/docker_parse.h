#ifndef _DOCKER_PARSE_H
#define _DOCKER_PARSE_H

#include "export.h"
#include "docker_types.h"

namespace asl {
    class Var;
}

namespace docker_cpp
{
    void DOCKER_CPP_API parse(const asl::Var &in, ImageList &out);
    void DOCKER_CPP_API parse(const asl::Var &in, DeletedImageList &out);
    void DOCKER_CPP_API parse(const asl::Var &in, PruneInfo &out);
    void DOCKER_CPP_API parse(const asl::Var &in, ContainerList &out);
    void DOCKER_CPP_API parse(const asl::Var &in, Port &out);
    void DOCKER_CPP_API parse(const asl::Var &in, NetworkSettings &out);
    void DOCKER_CPP_API parse(const asl::Var &in, VersionInfo &out);
    void DOCKER_CPP_API parse(const asl::Var &in, WaitInfo &out);
    void DOCKER_CPP_API parse(const asl::Var &in, ExecInfo &out);
    void DOCKER_CPP_API parse(const asl::Var &in, VolumeList &out);
    void DOCKER_CPP_API parse(const asl::Var &in, VolumeInfo &out);
    void DOCKER_CPP_API parse(const asl::Var &in, DeletedVolumesInfo &out);
} // namespace docker_cpp


#endif //_DOCKER_PARSE_H