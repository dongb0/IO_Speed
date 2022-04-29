#include <random>

#include "io_base.h"
#include "page_cache.h"
#include "direct.h"
#include "mmap.h"
using namespace std;

// Page Cache I/O
void pageCacheWrite400MB(string dirPath, string filePrefix){
    printf("Begin Page Cache Write...\n");
    PageCacheMode writer;
    int batch = 100, fileSize = 4*MB; // write [batch*fileSize] data to disk
    int count = 0;

    string fileName = filePrefix + to_string(0);
    auto start = writer.startTime();
    for(int i = 0; i < batch; i++){
        count += writer.Write(fileName, dirPath, fileSize);
    }
    auto end = writer.endTime();
    printf("Page Cache Write %.2f MB\n", (double)count/MB);
    writer.printDuration(start, end);
}

void pageCacheReadWrite400MB(string dirPath, string filePrefix){
    printf("Begin Page Cache Read/Write Operation...\n");
    PageCacheMode writer;
    int batch = 100, fileSize = 4*MB; // write [batch*fileSize] data to disk
    int rcount = 0, wcount = 0;

    string fileName = filePrefix + to_string(0);
    auto start = writer.startTime();
    for(int i = 0; i < batch; i++){
        wcount += writer.Write(fileName, dirPath, fileSize);
    }

    for(int i = 0; i < batch; i++){
        rcount += writer.Read(fileName, dirPath, fileSize);
    }

    auto end = writer.endTime();
    printf("Page Cache Write %.2f MB, Read %.2fMB\n", (double)wcount/MB, (double)rcount/MB);
    writer.printDuration(start, end);
}

void pageCacheRandom(string dirPath, string filePrefix, double possibility) {
    printf("Begin Page Cache Read/Write Operation(%.2f W, %.2f R)...\n", possibility, 1-possibility);
    PageCacheMode writer;
    int batch = 100, fileSize = 4*MB;
    int rcount = 0, wcount = 0;
    string fileName = filePrefix + to_string(0);
    
    random_device r;
    default_random_engine r_engine(r());
    uniform_real_distribution<double> randNum(0.0, 1.0);
    
    auto start = writer.startTime();
    for(int i = 0; i < batch; i++){
        if(randNum(r_engine) < possibility) {
            wcount += writer.Write(fileName, dirPath, fileSize);
        } else {
            rcount += writer.Read(fileName, dirPath, fileSize);
        }
    }

    auto end = writer.endTime();
    printf("Page Cache Random R/W  Write %.2f MB, Read %.2fMB\n", (double)wcount/MB, (double)rcount/MB);
    writer.printDuration(start, end);
}

// Direct I/O
void directWrite400MB(string dirPath, string filePrefix){
    printf("Begin Direct Write...\n");
    int batch = 100, fileSize = 4*MB; // write [batch*fileSize] data to disk
    int count = 0;
    
    DirectMode writer(fileSize);
    string fileName = filePrefix + to_string(0);
    auto start = writer.startTime();
    for(int i = 0; i < batch; i++){
        count += writer.Write(fileName, dirPath, fileSize);
    }
    auto end = writer.endTime();
    printf("Direct Write %.2f MB\n", (double)count/MB);
    writer.printDuration(start, end);
}

void directReadWrite(string dirPath, string filePrefix) {
    printf("Begin Direct Read/Write...\n");
    int batch = 100, fileSize = 4*MB;
    int rcount = 0, wcount = 0;
    DirectMode writer(fileSize);

    string fileName = filePrefix + to_string(0);
    auto start = writer.startTime();
    for(int i = 0; i < batch; i++){
        wcount += writer.Write(fileName, dirPath, fileSize);
    }
    for(int i = 0; i < batch; i++){
        rcount += writer.Read(fileName, dirPath, fileSize);
    }
    auto end = writer.endTime();
    printf("Direct Write %.2f MB, Read %.2fMB\n", (double)wcount/MB, (double)rcount/MB);
    writer.printDuration(start, end);
}

void directRandomRW(string dirPath, string filePrefix, double possibility){
    printf("Begin Direct Random R/W Operation(%.2f W, %.2f R)...\n", possibility, 1-possibility);
    int batch = 100, fileSize = 4*MB;
    int rcount = 0, wcount = 0;
    DirectMode writer(fileSize);

    random_device r;
    default_random_engine r_engine(r());
    uniform_real_distribution<double> randNum(0.0, 1.0);

    string fileName = filePrefix + to_string(0);
    auto start = writer.startTime();
    for(int i = 0; i < batch; i++){
        if(randNum(r_engine) < possibility) {
            wcount += writer.Write(fileName, dirPath, fileSize);
        } else {
            rcount += writer.Read(fileName, dirPath, fileSize);
        }
    }
    auto end = writer.endTime();
    printf("Direct Random R/W %.2f MB, Read %.2fMB\n", (double)wcount/MB, (double)rcount/MB);
    writer.printDuration(start, end);
}

// mmap
void mmapWrite(string dirPath, string filePrefix){
    printf("Begin Mmap Write...\n");
    int batch = 100, fileSize = 4*MB;
    int wcount = 0;
    MmapMode writer;

    string fileName = filePrefix + to_string(0);
    auto start = writer.startTime();
    for(int i = 0; i < batch; i++){
        wcount += writer.Write(fileName, dirPath, fileSize);
    }
    auto end = writer.endTime();
    printf("Mmap Write %.2f MB\n", (double)wcount/MB);
    writer.printDuration(start, end);
}

void mmapReadWrite(string dirPath, string filePrefix){
    printf("Begin Mmap Read/Write...\n");
    int batch = 100, fileSize = 4*MB;
    int rcount = 0, wcount = 0;
    MmapMode writer;

    string fileName = filePrefix + to_string(0);
    auto start = writer.startTime();
    for(int i = 0; i < batch; i++){
        wcount += writer.Write(fileName, dirPath, fileSize);
    }
    for(int i = 0; i < batch; i++){
        rcount += writer.Read(fileName, dirPath, fileSize);
    }
    auto end = writer.endTime();
    printf("Mmap Write %.2f MB, Read %.2fMB\n", (double)wcount/MB, (double)rcount/MB);
    writer.printDuration(start, end);
}

void mmapRandom(string dirPath, string filePrefix, double possibility){
    printf("Begin Mmap Random R/W Operation(%.2f W, %.2f R)...\n", possibility, 1-possibility);
    int batch = 100, fileSize = 4*MB;
    int rcount = 0, wcount = 0;
    MmapMode writer;

    random_device r;
    default_random_engine r_engine(r());
    uniform_real_distribution<double> randNum(0.0, 1.0);

    string fileName = filePrefix + to_string(0);
    auto start = writer.startTime();
    for(int i = 0; i < batch; i++){
        if(randNum(r_engine) < possibility) {
            wcount += writer.Write(fileName, dirPath, fileSize);
        } else {
            rcount += writer.Read(fileName, dirPath, fileSize);
        }
    }
    auto end = writer.endTime();
    printf("Mmap Write %.2f MB, Read %.2fMB\n", (double)wcount/MB, (double)rcount/MB);
    writer.printDuration(start, end);
}

// test wrapper 
void TestPageCache(){
    printf("----------------\n");
    string dir = "tmp/", filePrefix = "testFile-PC-W-";
    pageCacheWrite400MB(dir, filePrefix);
    printf("\n");

    filePrefix = "testFile-PC-RW-";
    pageCacheReadWrite400MB(dir, filePrefix);
    printf("\n");

    filePrefix = "testFile-PC-Random-";
    pageCacheRandom(dir, filePrefix, 0.8);
    printf("\n");

    pageCacheRandom(dir, filePrefix, 0.5);
    printf("\n");

    pageCacheRandom(dir, filePrefix, 0.2);
    printf("\n");

    pageCacheRandom(dir, filePrefix, 0.0);
    printf("\n");
}

void TestDirectIO(){
    printf("----------------\n");
    string dir = "tmp/", filePrefix = "testFile-D-";
    directWrite400MB(dir, filePrefix);
    printf("\n");

    filePrefix = "testFile-D-RW-";
    directReadWrite(dir, filePrefix);
    printf("\n");

    filePrefix = "testFile-D-Random-";
    directRandomRW(dir, filePrefix, 0.8);
    printf("\n");

    directRandomRW(dir, filePrefix, 0.5);
    printf("\n");

    directRandomRW(dir, filePrefix, 0.2);
    printf("\n");

    directRandomRW(dir, filePrefix, 0.0);
    printf("\n");
}

void TestMmap(){
    printf("----------------\n");
    string dir = "tmp/", filePrefix = "testFile-mmap-";
    mmapWrite(dir, filePrefix);
    printf("\n");

    filePrefix = "testFile-mmap-RW-";
    mmapReadWrite(dir, filePrefix);
    printf("\n");

    filePrefix = "testFile-mmap-Random-";
    mmapRandom(dir, filePrefix, 0.8);
    printf("\n");

    mmapRandom(dir, filePrefix, 0.5);
    printf("\n");

    mmapRandom(dir, filePrefix, 0.2);
    printf("\n");

    mmapRandom(dir, filePrefix, 0.0);
    printf("\n");
}

int main(){
    TestPageCache();
    TestDirectIO();
    TestMmap();
    return 0;
}