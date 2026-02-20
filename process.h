#include <vector>
#include <string>
using namespace std;


// Struct that represents a CPU process
struct Process {
    int index;
    int arrivalTime;
    int burstTime;
    int remainingBurstTime;
    int niceLevel;
    bool preempted = false;
};

// Struct for a scheduling algorithm test case
struct TestCase {
    string algorithm;
    int quantum = 0;  
    std::vector<Process> processes;
};

// Sorts processes by arrival time, breaking ties using process index
inline void sortProcesses(vector<Process> &processes) {
    for (int i = 0; i < processes.size() - 1; i++) {
        int minIndex = i;

        for (int j = i + 1; j < processes.size() ; j++) {
            if (processes[j].arrivalTime < processes[minIndex].arrivalTime ||
               (processes[j].arrivalTime == processes[minIndex].arrivalTime &&
                processes[j].index < processes[minIndex].index)) {
                minIndex = j;
            }
        }

        Process temp = processes[i];
        processes[i] = processes[minIndex];
        processes[minIndex] = temp;
    }
}

