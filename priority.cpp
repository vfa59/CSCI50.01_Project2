#include <iostream>
#include "process.h"
#include <vector>
using namespace std;

// Helper function to determine which process has a higher priority
bool hasHigherPriority(const Process& a, const Process& b) {

    if (a.niceLevel < b.niceLevel) {                        // compare nice levels first
        return true;
    }
    else if (a.niceLevel > b.niceLevel) {
        return false;
    }

    if (a.arrivalTime < b.arrivalTime) {                    // tie breaker if same nice levels, compare by arrival
        return true;
    }
    else if (a.arrivalTime > b.arrivalTime) {
        return false;
    }

    if (a.index < b.index) {                                // tie breaker if same nice levels and arrival time, compare by index
        return true;
    }
    else {
        return false;
    }
}

// Helper function to find process in Ready queue with hightest priority
inline int findHighestPriorityIndex(const vector<Process>& readyQueue) {

    int prioIndex = 0;

    for (int i = 1; i < readyQueue.size(); i++) {
        if (hasHigherPriority(readyQueue[i], readyQueue[prioIndex])) {
            prioIndex = i;
        }
    }

    return prioIndex;
}

void priority(vector<Process> processes) {
    sortProcesses(processes);                               // Initially sort processes by arrival time
    vector<Process> readyQueue;                             // initialize Ready queue

    int currentElapsedTime = 0;                             // keeps track of current elapsed time
    Process currentProcess;
    bool hasCurrentProcess = false;                         // determine if there is a current process executing
    int numProcesses = processes.size();            
    int finishedProcesses = 0;                              // tracks number of finished processes

    int processStartTime = 0;                               // time when the current process started running on the CPU
    int totalBurstTime = 0;                                 // tracs total CPU time used

    vector<int> turnaroundTimes(numProcesses, -1);          // store important metrics to be printed
    vector<int> waitingTimes(numProcesses, -1);
    vector<int> responseTimes(numProcesses, -1);
    vector<int> completionTimes(numProcesses, -1);


    while (finishedProcesses < numProcesses) {

        // Check which processes have arrived at the current time then add them to the Ready queue
        for (int i = 0; i < numProcesses; i++) {
            if (processes[i].arrivalTime == currentElapsedTime) {
                readyQueue.push_back(processes[i]);
            }
        }

        // If CPU is idle, then move current time forward
        if (!hasCurrentProcess && readyQueue.empty()) {
            currentElapsedTime++;
            continue;
        }

        // If there is a current process executing and Ready queue contains processes
        // then we should check if there is a process that takes higher priority than the one currently running
        if (hasCurrentProcess && !readyQueue.empty()) {

            int highestPrioIndex = findHighestPriorityIndex(readyQueue);            // find index of process in ready queue with highest priority

            if (hasHigherPriority(readyQueue[highestPrioIndex], currentProcess)) {      // If that process has higher priority than the current running process

                int runTime = currentElapsedTime - processStartTime;                    // compute how long the process has ran so far

                if (runTime > 0) {
                    cout << processStartTime << " " << (currentProcess.index + 1) << " " << runTime << endl;   // Output case for a preempted process
                }

                readyQueue.push_back(currentProcess);                   // Current process goes back to Ready queue to give way to higher priority process

                currentProcess = readyQueue[highestPrioIndex];              // switch to higher process and remove it (for now) from Ready queue
                readyQueue.erase(readyQueue.begin() + highestPrioIndex);

                processStartTime = currentElapsedTime;                      // Update process start time to start of new process

                totalBurstTime += runTime;                                  // update total cpu time used

                if (responseTimes[currentProcess.index] == -1) {                
                    responseTimes[currentProcess.index] = currentElapsedTime - currentProcess.arrivalTime;   // compute response time for the newly executed process
                }
            }
        }

        // If there is no executing process and Ready queue contains processes
        // then we simply run the process with the highest priority
        if(!hasCurrentProcess && !readyQueue.empty()) {
            int highestPrioIndex = findHighestPriorityIndex(readyQueue);

            currentProcess = readyQueue[highestPrioIndex];
            hasCurrentProcess = true;                                           // indicate that there is a current process executing
            readyQueue.erase(readyQueue.begin() + highestPrioIndex);            // remove it from the Ready queue for now

            processStartTime = currentElapsedTime;

            if (responseTimes[currentProcess.index] == -1) {
                responseTimes[currentProcess.index] = currentElapsedTime - currentProcess.arrivalTime;      // compute response time for the newly executed proces
            }
        }

        // Logic for execution of current process
        if (hasCurrentProcess) {
            currentProcess.remainingBurstTime -= 1;             // subtract its remaining time burst by 1 ns

            if (currentProcess.remainingBurstTime == 0) {                                                           // process is finished
                int runTime = (currentElapsedTime + 1) - processStartTime;                                          // get its total run time
                cout << processStartTime << " " << (currentProcess.index + 1) << " " << runTime << "X" << endl;     // output case for a finished process
                completionTimes[currentProcess.index] = currentElapsedTime + 1;
                hasCurrentProcess = false;
                finishedProcesses  += 1;                                                                            // update finished processes

                totalBurstTime += runTime;                                                                          // update total cpu time used

                int turnaround = (currentElapsedTime + 1) - currentProcess.arrivalTime;                             // turnaround time = completion time - process arrival time
                turnaroundTimes[currentProcess.index] = turnaround;
                waitingTimes[currentProcess.index] = turnaround - currentProcess.burstTime;                         // waiting time = turnaround time - process arrival time
            }
        }

         currentElapsedTime += 1;                                   // update current elapsed time
    }

    double cpuUtil = ((double) totalBurstTime / currentElapsedTime) * 100;          // cpu utilization: total burst time / total elapsed time
    double throughput = (double) numProcesses / currentElapsedTime;                  // throughput: # finished processes / total elapsed time

    //print total elapsed time, cpu burst time, cpu util, and throughput
    cout << "Total time elapsed: " << currentElapsedTime << "ns" << endl;                   
    cout << "Total CPU burst time: " << totalBurstTime << "ns" << endl;
    cout << "CPU Utilization: " << (int)cpuUtil << "%" << endl;
    cout << "Throughput: " << throughput << " processes/ns" << endl;

    cout << "Waiting times: " << endl;                                                 // print required metrics 
    int totalWaiting = 0;
    for (int i = 0; i < numProcesses; i++) {
        cout << " Process " << i+1 << ": " << waitingTimes[i] << "ns " << endl;
        totalWaiting += waitingTimes[i];
    }

    cout << "Average waiting time: " << ((double)totalWaiting / numProcesses) << "ns" << endl;

    cout << "Turnaround times: " << endl;
    int totalTurnaround = 0;
    for (int i = 0; i < numProcesses; i++) {
        cout << " Process " << i+1 << ": " << turnaroundTimes[i] << "ns " << endl;
        totalTurnaround += turnaroundTimes[i];
    }

    cout << "Average turnaround time: " << ((double)totalTurnaround / numProcesses) << "ns" << endl;

    cout << "Response times: " << endl;
    int totalResponse = 0;
    for (int i = 0; i < numProcesses; i++) {
        cout << " Process " << i+1 << ": " << responseTimes[i] << "ns " << endl;
        totalResponse += responseTimes[i];
    }

    cout << "Average response time: " << ((double)totalResponse / numProcesses) << "ns" << endl;
}

