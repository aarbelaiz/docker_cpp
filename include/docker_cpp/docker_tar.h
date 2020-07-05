#ifndef __DOCKER_TAR_
#define __DOCKER_TAR_

#include <archive.h>
#include <archive_entry.h>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <string>

#include <asl/Directory.h>

#ifdef __linux__
#define _FILE_OFFSET_BITS 64
#endif

namespace docker_cpp {

    static void fileToArchive(struct archive *a, const char *name, const char* pathname)
    {
        char buff[8192]; //8 MB blocks
        size_t bytes_read;
        struct archive *ard;
        struct archive_entry *entry;
        int fd;

        ard = archive_read_disk_new();
        archive_read_disk_set_standard_lookup(ard);
        entry = archive_entry_new();
        fd = open(name, O_RDONLY);
        if (fd < 0) return;
        archive_entry_copy_pathname(entry, pathname);
        archive_read_disk_entry_from_file(ard, entry, fd, NULL);
        int r = archive_write_header(a, entry);
        if (r < ARCHIVE_OK) { std::cout <<  archive_error_string(a); }
        while ((bytes_read = read(fd, buff, sizeof(buff))) > 0)
            archive_write_data(a, buff, bytes_read);
        archive_write_finish_entry(a);
        close(fd);
        archive_read_free(ard);
        archive_entry_free(entry);
    }

    static void recursiveFileToArchive(struct archive *a, asl::Directory dir, asl::String pathname)
    {
        for(asl::File& file : dir.files()) {
            asl::String absFilepath = asl::Path(file.path()).absolute();
            asl::Path archivhePath = pathname + file.name();
            //std::cout << "FILESYSTEM: " << *absFilepath << "\n";
            //std::cout << "ARCHIVE: " << *archivhePath << "\n";
            fileToArchive(a, *absFilepath, *archivhePath);
        }

        for (auto& d : dir.subdirs()) {
            //std::cout << "PATHNAME: " << *pathname << std::endl;
            recursiveFileToArchive(a, d, pathname + d.name() + "/");
        }
    }

    static long long directorySize(const std::string &dirPath)
    {
        long long totalSize = 0;
        std::vector<asl::Directory> q;
        q.emplace_back(asl::String(dirPath.c_str()));
        while (!q.empty())
        {
            asl::Directory dir = q.back();
            q.pop_back();
            for(auto& f : dir.files()) { totalSize += f.size(); }
            for(auto& d : dir.subdirs()) { q.push_back(d); }
        }
        return totalSize;
    }

    static void createTarInMemory(const std::string &path, void *buffer, size_t size, size_t &used)
    {
        struct archive *a;
        struct archive_entry *entry;
        struct stat st;
        int r;

        asl::Directory dir(path.c_str());
        if (dir.exists()) {
            a = archive_write_new();
            r = archive_write_add_filter_gzip(a);
            if (r < ARCHIVE_OK) { std::cout <<  archive_error_string(a); }
            r = archive_write_set_format_ustar(a);
            if (r < ARCHIVE_OK) { std::cout <<  archive_error_string(a); }
            r = archive_write_open_memory(a, buffer, size, &used);
            if (r < ARCHIVE_OK) { std::cout <<  archive_error_string(a); }

            // Recursively add files to archive
            recursiveFileToArchive(a, dir, "");

            archive_write_close(a);
            archive_write_free(a);
        }else{
            std::cout << "Error: provided context path is not a directory\n!";
        }
    }

    static asl::Array<byte> createTarWithTmpFile(const std::string &path)
    {
        struct archive *a;
        struct archive_entry *entry;
        struct stat st;
        int r;

        asl::Directory dir(path.c_str());
        if (dir.exists()) {
            a = archive_write_new();
            r = archive_write_add_filter_gzip(a);
            if (r < ARCHIVE_OK) { std::cout <<  archive_error_string(a); }
            r = archive_write_set_format_ustar(a);
            if (r < ARCHIVE_OK) { std::cout <<  archive_error_string(a); }

            // Create a temporary file 
            asl::String tmpFilePath = asl::File::temp(".tar.gz").path();
            int fd = open(*tmpFilePath, O_WRONLY);
            r = archive_write_open_fd(a, fd);

            // Recursively add files to archive
            recursiveFileToArchive(a, dir, "");
            
            // Close archive
            archive_write_close(a);
            archive_write_free(a);
            close(fd);
            sync();

            // Read tmp archive into buffer
            asl::File f(tmpFilePath, asl::File::READ);
            asl::Array<byte> context = f.content();
            f.close();
            f.remove();
            return context;
        }else{
            std::cout << "Error: provided context path is not a directory\n!";
            return asl::Array<byte>();
        }
    }

}

#endif //__DOCKER_TAR_