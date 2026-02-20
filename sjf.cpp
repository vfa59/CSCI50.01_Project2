#include <iostream>
#include "process.h"
#include <vector>
using namespace std;

// Helper function used to find the next process in the Ready queue with shortest burst time    
int findShortestBurstIndex(vector<Process>& readyQueue) {
    int shortestIndex = 0;                                      // index to track process with shortest burst time                          
    for (int i = 1; i < readyQueue.size(); i++) {
        Process &a = readyQueue[i];                             // compare process to the process with shortest burst so far
        Process &b = readyQueue[shortestIndex];

        if (a.burstTime < b.burstTime) {
            shortestIndex = i;                                  // update shortest index found so far
        }
        else if (a.burstTime == b.burstTime) {                  // tie breaker for when two processes have the same burst time
            if (a.arrivalTime < b.arrivalTime) {                // use arrival time to break the tie
                shortestIndex = i;
            } 
            else if (a.arrivalTime == b.arrivalTime) {          // if there is still a tie (same burst time and arrival time)
                if (a.index < b.index) {                        // use index to break the tie
                     shortestIndex = i;
                }
            }
        }
    }
    return shortestIndex;
}

void sjf(vector<Process> processes) { 
    sortProcesses(processes);                           // Initially sort processes by arrival time
    vector<Process> readyQueue;                         // Initialize ready queue
    int currentElapsedTime = 0;                         // keep track of elapsed time in scheduler
    Process currentProcess;                             // current process to execute

    int numProcesses = processes.size(); 
    int finishedProcesses = 0; 
    int totalBurstTime = 0;                                 // keeps track of total cpu time used

    vector<bool> queued(numProcesses, false);               // keeps track of which processes have already been queued
    vector<int> turnaroundTimes(numProcesses, -1);          // store important metrics to be printed
    vector<int> waitingTimes(numProcesses, -1); 
    vector<int> responseTimes(numProcesses, -1); 

    while (finishedProcesses < numProcesses) { 

        // check for processes that have arrived at the current time and marked them as queued
        for (int i = 0; i < numProcesses; i++) { 
            if (processes[i].arrivalTime <= currentElapsedTime && !queued[processes[i].index]) { 
                readyQueue.push_back(processes[i]); queued[processes[i].index] = true; 
            } 
        }
        
        // if ready queue is empty, move current time forward to the next arriving queue
        if (readyQueue.empty()) { 
            for (int i = 0; i < numProcesses; i++) { 
                if (!queued[processes[i].index]) { 
                    currentElapsedTime = processes[i].arrivalTime; break; 
                } 
            } 
            continue; 
        } 

        int shortestBurstIndex = findShortestBurstIndex(readyQueue);                // find the process with the shortest burst time in ready queue

        currentProcess = readyQueue[shortestBurstIndex];                            // set current process to that one
        readyQueue.erase(readyQueue.begin() + shortestBurstIndex);                  // remove current process from ready queue since it will now execute

        int firstResponseTime = currentElapsedTime;                                 // track first response time and completion time for future computation      
        int completionTime = currentElapsedTime + currentProcess.burstTime; 

        cout << firstResponseTime << " " << currentProcess.index + 1 << " " << currentProcess.burstTime << "X" << endl;     // output current time, process index and cpu time used

        currentElapsedTime += currentProcess.burstTime;                             // update current elapsed time and total cpu time used
        totalBurstTime += currentProcess.burstTime; 
        
        if (responseTimes[currentProcess.index] == -1) { 
            responseTimes[currentProcess.index] = firstResponseTime - currentProcess.arrivalTime;       // response time = first response time - process arrival time
        } 
        
        int turnaround = completionTime - currentProcess.arrivalTime;                                   // turnaround time = completion time - process arrival time
        turnaroundTimes[currentProcess.index] = turnaround; 
        waitingTimes[currentProcess.index] = turnaround - currentProcess.burstTime; 
        finishedProcesses += 1;                                                                         // update number of finished processes
    } 
    
    double cpuUtil = ((double) totalBurstTime / currentElapsedTime) * 100;                               // cpu utilization: total burst time / total elapsed time
    double throughput = (double) numProcesses / currentElapsedTime;                                       // throughput: # finished processes / total elapsed time

    // print total elapsed time, cpu burst time, cpu util, and throughput
    cout << "Total time elapsed: " << currentElapsedTime << "ns" << endl;                   
    cout << "Total CPU burst time: " << totalBurstTime << "ns" << endl;
    cout << "CPU Utilization: " << (int)cpuUtil << "%" << endl;
    cout << "Throughput: " << throughput << " processes/ns" << endl;

    cout << "Waiting times: " << endl;                                                           // print required metrics 
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
