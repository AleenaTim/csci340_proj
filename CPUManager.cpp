#include "CPUManager.h"

CPUManager::CPUManager(){

}
void CPUManager::AddToReadyQueue(int pid){
    readyQueue.push_back(pid);
}
void CPUManager::RemoveFromReadyQueue(int pid){
    readyQueue.pop_front();
}
bool CPUManager::getActivity(){   // returns bool value if CPU is handling processes in ready queue
    return readyQueue.empty();
}
std::deque<int> CPUManager::getReadyQueue(){
    return readyQueue;
}
int CPUManager::getPid(){
    return pid;
}