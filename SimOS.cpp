#include "SimOS.h"
#include "CPUManager.h"
#include "DiskManager.h"
#include "MemoryManager.h"

SimOS::SimOS(int numberOfDisks, unsigned long long amountOfRAM, unsigned int pageSize)
{
    numberOfDisks = 0;
    amountOfRAM = 0;
    pageSize = 0;
}
void SimOS::NewProcess()
{
    cpu.AddToReadyQueue(pid++);
    ram.AddNewProcess(pid);
}
void SimOS::SimFork()
{
    if (cpu == NO_PROCESS)
        throw std::logic_error("CPU is idle");
    cpu.AddToReadyQueue(pid++);
}
void SimOS::SimExit()
{
}
void SimOS::SimWait()
{
}
void SimOS::TimerInterrupt()
{
}
/*
Currently running process requests to read the specified file from the disk with a given number. The process issuing disk reading requests immediately stops using the CPU, even if the ready-queue is empty
*/
void SimOS::DiskReadRequest(int diskNumber, std::string fileName)
{
}
/*
A disk with a specified number reports that a single job is completed. The served process should return to the ready-queue
*/
void SimOS::DiskJobCompleted(int diskNumber)
{
}
/*
Currently running process wants to access the specified logical memory address. System makes sure the corresponding page is loaded in the RAM. If the corresponding page is already in the RAM, its “recently used” information is updated
*/
void SimOS::AccessMemoryAddress(unsigned long long address)
{
}
/*
GetCPU returns the PID of the process currently using the CPU. If CPU is idle it returns NO_PROCESS
*/
int SimOS::GetCPU()
{
    if (cpu.getActivity())
        return cpu.getPid();
    return NO_PROCESS;
}
/*
GetReadyQueue returns the std::deque with PIDs of processes in the ready-queue where element in front corresponds start of the ready-queue
*/
std::deque<int> SimOS::GetReadyQueue()
{
    return cpu.getReadyQueue();
}
/*
GetMemory returns MemoryUsage vector describing all currently used frames of RAM. Remember, Terminated “zombie” processes don’t use memory, so they don’t contribute to memory usage.
MemoryItems appear in the MemoryUsage vector in the order they appear in memory (from low addresses to high)
*/
MemoryUsage SimOS::GetMemory()
{
    return MemoryManager.getMemory();
}
/*
GetDisk returns an object with PID of the process served by specified disk and the name of the file read for that process. If the disk is idle, GetDisk returns the default FileReadRequest object (with PID 0 and empty string in fileName)
*/
FileReadRequest SimOS::GetDisk(int diskNumber)
{
}
/*
GetDiskQueue returns the I/O-queue of the specified disk starting from the “next to be served” process
*/
std::deque<FileReadRequest> SimOS::GetDiskQueue(int diskNumber)
{
}