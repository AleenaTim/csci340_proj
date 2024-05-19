#ifndef CPUMANAGER_H
#define CPUMANAGER_H
#include <iostream>
#include <deque>

class CPUManager
{
public:
    CPUManager();
    void AddToReadyQueue(int pid);
    void RemoveFromReadyQueue(int pid);
    bool getActivity();
    std::deque<int> getReadyQueue();
    int getPid();

private:
    int pid;
    std::deque<int> readyQueue;
    int cpu;
};
#endif