#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <fcntl.h>
#include <unistd.h>
#include <cassert>
using namespace std::chrono;

const int KB = 1024;
const int MB = 1024 * KB;
const int SAMPLE_SIZE = 8 * KB;
const int MAX_BUFFER_SIZE = 8 * MB;
char globalBuffer[MAX_BUFFER_SIZE];
char cmpBuffer[MAX_BUFFER_SIZE];

class IOBase
{
private:

public:
    IOBase(){}
    ~IOBase(){}

    steady_clock::time_point startTime() {
        return steady_clock::now();
    }

    steady_clock::time_point endTime() {
        return steady_clock::now();
    }

    void printDuration(steady_clock::time_point start,
                        steady_clock::time_point end) {
        duration<double> d = end - start;
        printf("Time eplased: %f sec\n", d.count());
    }

    bool fileExist(std::string fileName, std::string path="./") {
        // TODO
        return false;
    }

    void fillBufferWithSize(char *buf, int size) {
        static std::string sampleNums = "123456789";
        int SSIZE = sampleNums.length();

        for(int i = 0; i < size; i++) {
            buf[i] = sampleNums[i%SSIZE];
        }
        buf[size-1] = '\0';
    }

    virtual int Write(std::string fileName, std::string path="./", int size=SAMPLE_SIZE) = 0;
    virtual int Read(std::string fileName, std::string path="./", int size=SAMPLE_SIZE) = 0;
};
