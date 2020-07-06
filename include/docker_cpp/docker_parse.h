#ifndef _DOCKER_PARSE_H
#define _DOCKER_PARSE_H

#include "docker_types.h"

namespace asl {
    class Var;
}

namespace docker_cpp
{
    void parse(const asl::Var &in, ImageList &out);
    void parse(const asl::Var &in, DeletedImageList &out);
    void parse(const asl::Var &in, PruneInfo &out);
    void parse(const asl::Var &in, ContainerList &out);
    void parse(const asl::Var &in, Port &out);
    void parse(const asl::Var &in, NetworkSettings &out);
    void parse(const asl::Var &in, VersionInfo &out);
    void parse(const asl::Var &in, WaitInfo &out);
    void parse(const asl::Var &in, ExecInfo &out);
    void parse(const asl::Var &in, DeletedVolumesInfo &out);
} // namespace docker_cpp


#endif //_DOCKER_PARSE_H