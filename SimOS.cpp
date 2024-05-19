#include "SimOS.h"
/*
The parameters specify number of hard disks in the simulated computer, amount of memory, and page size.
Disks, frame, and page enumerations all start from 0.
*/
SimOS::SimOS(int numberOfDisks, unsigned long long amountOfRAM, unsigned int pageSize)
    : numberOfDisks(numberOfDisks), amountOfRAM(amountOfRAM), pageSize(pageSize),
      nextPID(1), maxFrames(amountOfRAM / pageSize) {}

void SimOS::releaseProcessMemory(int PID)
{
    if (processMemory.find(PID) != processMemory.end())
    {
        for (int frame : processMemory[PID])
        {
            memoryFrameMap.erase(frame);
            memory.erase(std::remove_if(memory.begin(), memory.end(), [&](const MemoryItem &item)
                                        { return item.frameNumber == frame; }),
                         memory.end());
        }
        processMemory.erase(PID);
    }
}
/*
Creates a new process in the simulated system. The new process takes place in the ready-queue or immediately starts using the CPU.
Every process in the simulated system has a PID. Your simulation assigns PIDs to new processes starting from 1 and increments it by one for each new process. Do not reuse PIDs of the terminated processes.
*/
void SimOS::NewProcess()
{
    if (CPU == NO_PROCESS)
        CPU = nextPID++;
    else
        readyQueue.push_back(nextPID++);
}
/*
The currently running process forks a child. The child is placed in the end of the ready-queue.
*/
void SimOS::SimFork()
{
    if (CPU == NO_PROCESS)
        throw std::logic_error("CPU is idle");
    readyQueue.push_back(nextPID++);
}
/*
The process that is currently using the CPU terminates. Make sure you release the memory used by this process immediately. If its parent is already waiting, the process terminates immediately and the parent becomes runnable (goes to the ready-queue). If its parent hasn't called wait yet, the process turns into zombie.
To avoid the appearance of the orphans, the system implements the cascading termination. Cascading termination means that if a process terminates, all its descendants terminate with it.
*/
void SimOS::SimExit()
{
    if (CPU == NO_PROCESS)
        throw std::logic_error("CPU is idle");

    int pid = CPU;
    releaseProcessMemory(pid);

    if (!readyQueue.empty())
    {
        CPU = readyQueue.front();
        readyQueue.pop_front();
    }
    else
    {
        CPU = NO_PROCESS;
    }
}
void SimOS::SimWait()
{
    if (CPU == NO_PROCESS)
        throw std::logic_error("CPU is idle");
    // SimWait implementation - assuming immediate response for simplicity
}
void SimOS::TimerInterrupt()
{
    if (CPU == NO_PROCESS)
        throw std::logic_error("CPU is idle");

    readyQueue.push_back(CPU);
    CPU = readyQueue.front();
    readyQueue.pop_front();
}
/*
Currently running process requests to read the specified file from the disk with a given number. The process issuing disk reading requests immediately stops using the CPU, even if the ready-queue is empty
*/
void SimOS::DiskReadRequest(int diskNumber, std::string fileName)
{
    if (diskNumber < 0 || diskNumber >= numberOfDisks)
        throw std::out_of_range("Invalid disk number");
    if (CPU == NO_PROCESS)
        throw std::logic_error("CPU is idle");

    FileReadRequest request;
    request.PID = CPU;
    request.fileName = fileName;
    diskQueues[diskNumber].push_back(request);
    CPU = NO_PROCESS;
}
/*
A disk with a specified number reports that a single job is completed. The served process should return to the ready-queue
*/
void SimOS::DiskJobCompleted(int diskNumber)
{
    if (diskNumber < 0 || diskNumber >= numberOfDisks)
        throw std::out_of_range("Invalid disk number");

    if (!diskQueues[diskNumber].empty())
    {
        readyQueue.push_back(diskQueues[diskNumber].front().PID);
        diskQueues[diskNumber].pop_front();
    }
}
/*
Currently running process wants to access the specified logical memory address. System makes sure the corresponding page is loaded in the RAM. If the corresponding page is already in the RAM, its “recently used” information is updated
*/
void SimOS::AccessMemoryAddress(unsigned long long address)
{
    if (CPU == NO_PROCESS)
        throw std::logic_error("CPU is idle");

    unsigned long long pageNumber = address / pageSize;
    auto it = std::find_if(memory.begin(), memory.end(), [&](const MemoryItem &item)
                           { return item.pageNumber == pageNumber && item.PID == CPU; });

    if (it == memory.end())
    {
        if (memory.size() >= maxFrames)
        {
            memoryFrameMap.erase(memory.front().frameNumber);
            processMemory[memory.front().PID].erase(memory.front().frameNumber);
            memory.erase(memory.begin());
        }
        unsigned long long frameNumber = memory.size();
        memory.push_back({pageNumber, frameNumber, CPU});
        memoryFrameMap[frameNumber] = pageNumber;
        processMemory[CPU].insert(frameNumber);
    }
    else
    {
        // Update LRU information here if needed
    }
}
/*
GetCPU returns the PID of the process currently using the CPU. If CPU is idle it returns NO_PROCESS
*/
int SimOS::GetCPU()
{
    return CPU;
}
/*
GetReadyQueue returns the std::deque with PIDs of processes in the ready-queue where element in front corresponds start of the ready-queue
*/
std::deque<int> SimOS::GetReadyQueue()
{
    return readyQueue;
}
/*
GetMemory returns MemoryUsage vector describing all currently used frames of RAM. Remember, Terminated “zombie” processes don’t use memory, so they don’t contribute to memory usage.
MemoryItems appear in the MemoryUsage vector in the order they appear in memory (from low addresses to high)
*/
MemoryUsage SimOS::GetMemory()
{
    return memory;
}
/*
GetDisk returns an object with PID of the process served by specified disk and the name of the file read for that process. If the disk is idle, GetDisk returns the default FileReadRequest object (with PID 0 and empty string in fileName)
*/
FileReadRequest SimOS::GetDisk(int diskNumber)
{
    if (diskNumber < 0 || diskNumber >= numberOfDisks)
        throw std::out_of_range("Invalid disk number");

    if (!diskQueues[diskNumber].empty())
    {
        return diskQueues[diskNumber].front();
    }

    return FileReadRequest{};
}
/*
GetDiskQueue returns the I/O-queue of the specified disk starting from the “next to be served” process
*/
std::deque<FileReadRequest> SimOS::GetDiskQueue(int diskNumber)
{
    if (diskNumber < 0 || diskNumber >= numberOfDisks)
        throw std::out_of_range("Invalid disk number");
    return diskQueues[diskNumber];
}
