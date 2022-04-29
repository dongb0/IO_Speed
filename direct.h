#include <memory>
#include <cstring>

#include "io_base.h"

class DirectMode : public IOBase
{
private:
    char *buffer;
    int bufferSize = 0;
public:
    DirectMode(int size): bufferSize(size) {
        buffer = reinterpret_cast<char*>(aligned_alloc(512, bufferSize));
        if(buffer == nullptr) {
            std::cerr << "aligned_alloc failed" << std::endl;
            exit(-1);
        }
    }
    ~DirectMode(){
        delete []buffer;
    }

    int Write(std::string fileName, std::string path="./", int size=SAMPLE_SIZE){
        assert(size < MAX_BUFFER_SIZE);

        std::string fileStr = path+fileName;
        int fd = open(fileStr.c_str(), O_CREAT|O_WRONLY|O_DIRECT, 0655);
        if(fd == -1) {
            std::cerr << "open file " << fileStr << " failed" << std::endl;
            return 0;
        }
        
        fillBufferWithSize(buffer, size);
        int count = write(fd, buffer, size);

        // close(fd);
        return count < 0 ? 0 : count;
    }

    int Read(std::string fileName, std::string path="./", int size=SAMPLE_SIZE){        
        assert(size < MAX_BUFFER_SIZE);

        std::string fileStr = path+fileName;
        int fd = open(fileStr.c_str(), O_RDWR|O_DIRECT|O_SYNC);
        if(fd == -1) {
            std::cerr << "open file " << fileStr << " failed" << std::endl;
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

        // close(fd);
        return count < 0 ? 0 : count;
    }

};

