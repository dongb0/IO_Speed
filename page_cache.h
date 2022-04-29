#include <cstring>

#include "io_base.h"

class PageCacheMode : public IOBase
{
private:

public:
    PageCacheMode(){}
    ~PageCacheMode(){}

    int Write(std::string fileName, std::string path="./", int size = SAMPLE_SIZE){
        assert(size < MAX_BUFFER_SIZE);

        std::string fileStr = path+fileName;
        int fd = open(fileStr.c_str(), O_CREAT|O_WRONLY, 0655);
        if(fd == -1) {
            std::cerr << "open file(WRITE) " << fileStr << " failed" << std::endl;
            return 0;
        }
        
        fillBufferWithSize(globalBuffer, size);
        int count = write(fd, globalBuffer, size);
        close(fd);
        return count < 0 ? 0 : count;
    }
    
    int Read(std::string fileName, std::string path="./", int size=SAMPLE_SIZE){
        assert(size < MAX_BUFFER_SIZE);

        std::string fileStr = path+fileName;
        int fd = open(fileStr.c_str(), O_RDWR);
        if(fd == -1) {
            std::cerr << "open file(READ) " << fileStr << " failed" << std::endl;
            return 0;
        }
        
        int count = read(fd, globalBuffer, size);
        if(cmpBuffer[0] == '\0') {
            fillBufferWithSize(cmpBuffer, size);
        }

        if(strcmp(cmpBuffer, globalBuffer) != 0) {
            std::cerr << "mmap read get unexpected data" << std::endl;
            count = 0;
        }

        close(fd);
        return count < 0 ? 0 : count;
    }
};


