#include <iostream>
#include "process.h"
#include <vector>
using namespace std;

void fcfs(vector<Process> processes) {
    sortProcesses(processes);                               // Initially sort processes by arrival time
    int currentElapsedTime = 0;                             // keeps track of current elapsed time for scheduler
    int totalBurstTime = 0;                                 // keeps track of total cpu time used
    int numProcesses = processes.size();

    vector<int> turnaroundTimes(numProcesses, -1);          // store important metrics to be printed
    vector<int> waitingTimes(numProcesses, -1);
    vector<int> responseTimes(numProcesses, -1);

    double totalWaitingTime = 0;
    double totalTurnaroundTime = 0;
    double totalResponseTime = 0;

    Process currentProcess;

    for (int i = 0; i < numProcesses; i++) {

        currentProcess = processes[i];

        // Case if first process arrived at time > 0 ns
        if (currentElapsedTime < currentProcess.arrivalTime)
            currentElapsedTime = currentProcess.arrivalTime;
        
        int processStartTime = currentElapsedTime;                                  // start time of current process
        int completionTime = processStartTime + currentProcess.burstTime;           // completion time of current process           

        cout << currentElapsedTime << " " << (currentProcess.index + 1) << " " << currentProcess.burstTime << "X" << endl;      // print current time, process index, and cpu time used

        // compute turnaround, waiting, and response times then store the values
        int turnaroundTime = completionTime - currentProcess.arrivalTime;
        int waitingTime = turnaroundTime - currentProcess.burstTime;
        int responseTime = processStartTime - currentProcess.arrivalTime;

        turnaroundTimes[currentProcess.index] = turnaroundTime;
        waitingTimes[currentProcess.index] = waitingTime;
        responseTimes[currentProcess.index] = responseTime;

        // update totals for cpu time used, turnaround time, waiting time, and response time for future computation
        totalBurstTime += currentProcess.burstTime;
        totalTurnaroundTime += turnaroundTime;
        totalWaitingTime += waitingTime;
        totalResponseTime += responseTime;

        currentElapsedTime = completionTime;    // update current elapsed time
    }

    double cpuUtil = ((double)totalBurstTime / currentElapsedTime) * 100.0;          // cpu utilization: total burst time / total elapsed time
    double throughput = (numProcesses / (double)currentElapsedTime);                 // throughput: # finished processes / total elapsed time

    // print total elapsed time, cpu burst time, cpu util, and throughput
    cout << "Total time elapsed: " << currentElapsedTime << "ns" << endl;                   
    cout << "Total CPU burst time: " << totalBurstTime << "ns" << endl;
    cout << "CPU Utilization: " << (int)cpuUtil << "%" << endl;
    cout << "Throughput: " << throughput << " processes/ns" << endl;

    cout << "Waiting times: " << endl;                                              // print required metrics 
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

