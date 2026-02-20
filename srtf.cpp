#include <iostream>
#include "process.h"
#include <vector>
using namespace std;

void srtf(vector<Process> processes) {

    sortProcesses(processes);    

    int numProcesses = processes.size();

    
    for (int i = 0; i < numProcesses; i++) {
        processes[i].remainingBurstTime = processes[i].burstTime;
    }

    vector<int> turnaroundTimes(numProcesses, -1);  
    vector<int> waitingTimes(numProcesses, -1);     
    vector<int> responseTimes(numProcesses, -1);

    int currentElapsedTime = 0;
    int totalBurstTime = 0;
    int processesFinished = 0;

    int currentIdx = -1;        // keeps track of which process is currently running
    int burstStart = -1;


    while (processesFinished < numProcesses) {
        // find process w shortest remaining time
        int shortestIdx = -1;

        for (int i = 0; i < numProcesses; i++){
            // checks if the process has arrived and have remaining burst time left
            if (processes[i].arrivalTime <= currentElapsedTime && processes[i].remainingBurstTime > 0) {
                // compare to current shortest 
                if (shortestIdx == -1 || processes[i].remainingBurstTime < processes[shortestIdx].remainingBurstTime) {
                    shortestIdx = i;
                }
            }
        }

        // idle; no process ready
        if (shortestIdx == -1) {
            currentElapsedTime++;
            continue;
        }

        // get original index of ready process
        int originalIdx = processes[shortestIdx].index;

        // switching to another process
        if (shortestIdx != currentIdx) {
            // compute duration of previous process
            if (currentIdx != -1) {
                int duration = currentElapsedTime - burstStart;
                if (duration > 0) {
                    int prevOriginal = processes[currentIdx].index;
                    cout << burstStart << " " << (prevOriginal + 1) << " " << duration << endl;
                }
            }

            currentIdx = shortestIdx;
            burstStart = currentElapsedTime;

            // if first time the process is executed
            if (responseTimes[originalIdx] == -1) {
                responseTimes[originalIdx] = currentElapsedTime - processes[shortestIdx].arrivalTime;
            }
        }

        processes[shortestIdx].remainingBurstTime--;
        currentElapsedTime++;
        totalBurstTime++;

        // check if process has finished, print burst
        if (processes[shortestIdx].remainingBurstTime == 0) {
            int duration = currentElapsedTime - burstStart;
            cout << burstStart << " " << (originalIdx + 1) << " " << duration << "X" << endl;

            int completionTime = currentElapsedTime;
            turnaroundTimes[originalIdx] = completionTime - processes[shortestIdx].arrivalTime;
            waitingTimes[originalIdx] = turnaroundTimes[originalIdx] - processes[shortestIdx].burstTime;

            processesFinished++;
            currentIdx = -1;        // no process running
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