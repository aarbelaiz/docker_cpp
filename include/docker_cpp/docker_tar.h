#ifndef __DOCKER_TAR_
#define __DOCKER_TAR_

#include "export.h"

#include <asl/Directory.h>
#include <asl/String.h>

#include <string>

#ifdef __linux__
#define _FILE_OFFSET_BITS 64
#endif

struct archive;
namespace docker_cpp {

    void DOCKER_CPP_API fileToArchive(struct archive *a, const char *name, const char* pathname);
    void DOCKER_CPP_API recursiveFileToArchive(struct archive *a, asl::Directory dir, asl::String pathname);
    long long DOCKER_CPP_API directorySize(const std::string &dirPath);
    void DOCKER_CPP_API createTarInMemory(const std::string &path, void *buffer, size_t size, size_t &used);
    asl::Array<byte> DOCKER_CPP_API createTarWithTmpFile(const std::string &path);
}

#endif //__DOCKER_TAR_