#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "process.h"      
using namespace std;

void fcfs(std::vector<Process> processes);
void sjf(std::vector<Process> processes);
void srtf(std::vector<Process> processes);
void priority(std::vector<Process> processes);
void rr(std::vector<Process> processes, int quantum);

// This program reads test cases for different CPU scheduling algorithms and outputs Gantt charts and metrics.
// To provide input, redirect a text file containing test cases to the program:
// E.g. ./program < input.txt

int main() {
    int numTestCases;
    if (!(cin >> numTestCases)) return 0;

    // Loop through each test case, reading its processes and algorithm
    // then run the appropriate scheduling algorithm
    for (int i = 1; i <= numTestCases; ++i) {
        int numProcesses;
        string algorithm;
        
        if (!(cin >> numProcesses >> algorithm)) break;

        vector<Process> processList;
        int quantum = 0;

        // quantum only applies to round robin
        if (algorithm == "RR") {
            cin >> quantum;
        }

        // for each process, read its arrival time, burst time, and nice level
        for (int j = 0; j < numProcesses; ++j) {
            Process p;
            p.index = j; 
            cin >> p.arrivalTime >> p.burstTime >> p.niceLevel;
            
            p.remainingBurstTime = p.burstTime;                 
            p.preempted = false;                                // tracks if process has been preempted
            
            processList.push_back(p);
        }

        cout << i << " " << algorithm << std::endl;              // print test case # and algorithm

        // select appripriate algorithm to process input text
        if (algorithm == "FCFS") {
            fcfs(processList);
        } 
        else if (algorithm == "SJF") {
            sjf(processList, quantum);
        }
        else if (algorithm == "SRTF") {
            srtf(processList, quantum);
        }
        else if (algorithm == "P") {
            priority(processList, quantum);
        }
        else if (algorithm == "RR") {
            rr(processList, quantum);
        }
    }
    return 0;
}
