#include <sys/mman.h>
#include <cstring>

#include "io_base.h"

class MmapMode : public IOBase
{
private:
    // char *addr;
public:
    MmapMode(){}
    ~MmapMode(){}

    int Write(std::string fileName, std::string path="./", int size=SAMPLE_SIZE){
        assert(size < MAX_BUFFER_SIZE);

        std::string fileStr = path+fileName;
        int fd = open(fileStr.c_str(), O_CREAT|O_RDWR, 0655);
        if(fd == -1) {
            std::cerr << "open file " << fileStr << " failed" << std::endl;
            return 0;
        }

        // mmap every time we call Write(), so that we have the same system call as Page Cache I/O
        char* addr = (char*)mmap(nullptr, size, PROT_WRITE, MAP_SHARED, fd, 0);
        fallocate(fd, 0, 0, size);
        fillBufferWithSize(reinterpret_cast<char*>(addr), size);

        int writeCount = size;
        if(munmap(addr, size)) {
            std::cerr << "munmap error " << std::endl;
            writeCount = 0;
        }
        close(fd);
        return writeCount;
    }

    int Read(std::string fileName, std::string path="./", int size=SAMPLE_SIZE){
        assert(size < MAX_BUFFER_SIZE);

        std::string fileStr = path+fileName;
        int fd = open(fileStr.c_str(), O_RDWR);
        if(fd == -1) {
            std::cerr << "open file " << fileStr << " failed" << std::endl;
            return 0;
        }

        // generate the data we expected to get from file
        if(cmpBuffer[0] == '\0') {
            fillBufferWithSize(cmpBuffer, size);
        }

        char *addr = reinterpret_cast<char*>(mmap(nullptr, size, PROT_READ, MAP_SHARED, fd, 0));
        
        memcpy(globalBuffer, addr, size);
        munmap(addr, size);

        int readCount = size, statu = 0;
        if((statu = strcmp(cmpBuffer, globalBuffer)) != 0) {
            std::cerr << "mmap read get unexpected data, cmp:" << statu << std::endl;
            readCount = 0;
        }
        close(fd);
        return readCount;
    }

};