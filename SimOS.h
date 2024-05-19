/*
*/
#ifndef SimOS_H_
#define SimOS_H_
#include <iostream>
#include <queue>
#include <deque>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <unordered_set>
#include "../../../../msys64/mingw64/include/c++/12.1.0/bits/algorithmfwd.h"

struct FileReadRequest
{
    int PID{0}; // process id 
    std::string fileName{""};
};

struct MemoryItem
{
    unsigned long long pageNumber;
    unsigned long long frameNumber;
    int PID; // pid of the process using this frame of memory
};

using MemoryUsage = std::vector<MemoryItem>;

constexpr int NO_PROCESS{0};

class SimOS
{
public:
    SimOS(int numberOfDisks, unsigned long long amountOfRAM, unsigned int pageSize);
    void releaseProcessMemory(int PID);
    void NewProcess();
    void SimFork();
    void SimExit();
    void SimWait();
    void TimerInterrupt();
    void DiskReadRequest(int diskNumber, std::string fileName);
    void DiskJobCompleted(int diskNumber);
    void AccessMemoryAddress(unsigned long long address);
    int GetCPU();
    std::deque<int> GetReadyQueue();
    MemoryUsage GetMemory();
    FileReadRequest GetDisk(int diskNumber);
    std::deque<FileReadRequest> GetDiskQueue(int diskNumber);

private:
    MemoryUsage memory;
    int numberOfDisks, diskNumber;
    unsigned long long amountOfRAM;
    unsigned int pageSize;
    std::string fileName;
    int nextPID;
    std::deque<int> readyQueue;
    int CPU{0};
    std::unordered_map<int, std::deque<FileReadRequest>> diskQueues;
    std::unordered_map<int, std::unordered_set<int>> processMemory;
    std::unordered_map<int, int> memoryFrameMap;
    unsigned long long maxFrames;
};
#endif