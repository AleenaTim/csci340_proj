#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H
#include "SimOS.cpp"
class MemoryManager
{
public:
    MemoryManager();
    void AddNewProcess(int pid);
    MemoryUsage getMemory();
private:
    int pid;
    MemoryUsage memory;
};
#endif