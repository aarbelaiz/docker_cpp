#ifndef __DOCKER_TAR_
#define __DOCKER_TAR_

#include <string>

#include <asl/Directory.h>
#include <asl/String.h>

#ifdef __linux__
#define _FILE_OFFSET_BITS 64
#endif

struct archive;
namespace docker_cpp {

    void fileToArchive(struct archive *a, const char *name, const char* pathname);
    void recursiveFileToArchive(struct archive *a, asl::Directory dir, asl::String pathname);
    long long directorySize(const std::string &dirPath);
    void createTarInMemory(const std::string &path, void *buffer, size_t size, size_t &used);
    asl::Array<byte> createTarWithTmpFile(const std::string &path);
}

#endif //__DOCKER_TAR_