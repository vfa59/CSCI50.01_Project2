#include <iostream>
#include "process.h"
#include <deque>
#include <vector>
using namespace std;

void rr(vector<Process> processes, int quantum) {
    sortProcesses(processes);                        // Initially sort processes by arrival time

    deque<Process> readyQueue;
    int currentElapsedTime = 0;
    int numProcesses = processes.size();
    int processesQueued = 0;                        // tracks which processes have been accepted by ready queue
    int processesFinished = 0;                      // tracks which processes have finished execution
    Process currentProcess;                         // variable to keep track of current process

    int totalBurstTime = 0;
    
    vector<int> turnaroundTimes(numProcesses, -1);  
    vector<int> waitingTimes(numProcesses, -1);     
    vector<int> responseTimes(numProcesses, -1);

    while (processesFinished < numProcesses) {

         // checks if queue is empty, then moves time to next process arrival
        if (readyQueue.empty() && processesQueued < numProcesses) {
            if (currentElapsedTime < processes[processesQueued].arrivalTime) {
                currentElapsedTime = processes[processesQueued].arrivalTime;
            }
        }

        // check which processes have arrived at the current time
        for (int i = processesQueued; i < numProcesses; i++) {
            if (processes[i].arrivalTime <= currentElapsedTime) {
                int insertIdx = readyQueue.size();                                      // index to keep track of preempted processes
                for (int j = 0; j < readyQueue.size(); j++) {
                    if (readyQueue[j].preempted) {
                        insertIdx = j;
                        break;
                    }
                }
                readyQueue.insert(readyQueue.begin() + insertIdx, processes[i]);         // insert new arrivals in front of preempted processes
                processesQueued++;
            } else {
                break;
            }
        }

        // if queue empty even after new arrivals, wait for next arrival
        if (readyQueue.empty()) continue;
        
        currentProcess = readyQueue.front();                                                // execute the process at the front of the ready queue
        readyQueue.pop_front();                                                             // pop front process so it can be sent back to end of queue (or removed from queue if finished)
        
        if (responseTimes[currentProcess.index] == -1) {
            responseTimes[currentProcess.index] = currentElapsedTime - currentProcess.arrivalTime;     // store response time for process
        }

        int timeUsed;                                                                       // keeps track of cpu time allocated to process

        if (currentProcess.remainingBurstTime < quantum) {
            timeUsed = currentProcess.remainingBurstTime;                                   // if process finished before whole quantum    
        } else {
            timeUsed = quantum;                                                             // if process used whole quantum
        }       
                 
        cout << currentElapsedTime << " " << (currentProcess.index + 1) << " " << timeUsed;         // output current time, process index, and cpu time used in ns

        currentProcess.remainingBurstTime -= timeUsed;                                       // update remaining burst time for current process
        currentElapsedTime += timeUsed;                                                             // update scheduler time
        totalBurstTime += timeUsed;                                                          // update total CPU activity

        if (currentProcess.remainingBurstTime == 0) {                                        // current process is finished
            cout << "X";
            processesFinished++;

            int turnaround = currentElapsedTime - currentProcess.arrivalTime;                // process finished, compute turnaround time here (completion time - arrival time)
            turnaroundTimes[currentProcess.index] = turnaround;
            waitingTimes[currentProcess.index] = turnaround - currentProcess.burstTime;      // waiting time: turnaround time - execution time
        }
        cout << endl;

        // If process is not finished, mark it as preempted and send to end of queue
        if (currentProcess.remainingBurstTime > 0) {
            currentProcess.preempted = true;
            readyQueue.push_back(currentProcess);
        }
    }

    double cpuUtil = ((double) totalBurstTime / currentElapsedTime) * 100;                  // cpu utilization: total burst time / total elapsed time
    double throughput = (double) numProcesses / currentElapsedTime;                         // throughput: # finished processes / total elapsed time    

    cout << "Total time elapsed: " << currentElapsedTime << "ns" << endl;                   // print required metrics 
    cout << "Total CPU burst time: " << totalBurstTime << "ns" << endl;
    cout << "CPU Utilization: " << cpuUtil << "%" << endl;
    cout << "Throughput: " << throughput << " processes/ns" << endl;

    cout << "Waiting times: " << endl;
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
