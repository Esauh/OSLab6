#include<stdio.h> 
#include<limits.h>
#include<stdlib.h>
#include "process.h"
#include "util.h"
//Esau Hutcherson


void findWaitingTimeRR(ProcessType plist[], int n,int quantum) 
{
    int i = 0;
    int t = 0;
    int rem_bt[n];

  for (i = 0; i < n; i++){
    rem_bt[i] = plist[i].bt;
    plist[i].wt = 0; 
  }
  for(i = 0; i < n; i++){
    if (rem_bt[i] > 0){
      if (rem_bt[i] > quantum){
        t = t + quantum; 
        rem_bt[i] -= quantum;
      }
      else{
        t = t + rem_bt[i]; 
        plist[i].wt = t - plist[i].bt; 
        rem_bt[i] = 0; 
      }
    }
  }
} 


void findWaitingTimeSJF(ProcessType plist[], int n)
{ 
  int pro_counts = 0;
  int i =0;
  int minimum = 0;
  int curr = 0;
  int comp_time = 0; 
  int rem_bt[n];
  
    for(i = 0; i < n; i++){
        rem_bt[i] = plist[i].bt;
    }
    while(pro_counts < n){ 
      for(i = 1; i < n; i++){
        if (rem_bt[i] < rem_bt[minimum]){
          minimum = i;
        }
      }  
      rem_bt[minimum] -= 1; 
  
      if (rem_bt[minimum] == 0){
        pro_counts = pro_counts + 1;    
        comp_time = curr + 1; 
        plist[minimum].wt = comp_time - plist[minimum].bt; 
        rem_bt[minimum] = 9999;
      } 
    curr = curr + 1;  
    } 
} 

void findWaitingTime(ProcessType plist[], int n)
{ 
    plist[0].wt = 0 +  plist[0].art; 
  
    for (int  i = 1; i < n ; i++ ) 
        plist[i].wt =  plist[i-1].bt + plist[i-1].wt; 
} 
  
void findTurnAroundTime( ProcessType plist[], int n)
{ 
    for (int  i = 0; i < n ; i++) 
        plist[i].tat = plist[i].bt + plist[i].wt; 
} 
  
int my_comparer(const void *this, const void *that)
{

    if (((ProcessType*)this)->pri < ((ProcessType*)that)->pri){ 
      return 1;
    }
  return 0;
} 

void findavgTimeFCFS( ProcessType plist[], int n) 
{ 
    findWaitingTime(plist, n); 
  
    findTurnAroundTime(plist, n); 
  
    printf("\n*********\nFCFS\n");
}

//Function to calculate average time 
void findavgTimeSJF( ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeSJF(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nSJF\n");
}

//Function to calculate average time 
void findavgTimeRR( ProcessType plist[], int n, int quantum) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeRR(plist, n, quantum); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nRR Quantum = %d\n", quantum);
}

//Function to calculate average time 
void findavgTimePriority( ProcessType plist[], int n) 
{ 
    qsort(plist, n, sizeof(ProcessType), my_comparer);
    findWaitingTime(plist, n); 
    findTurnAroundTime(plist, n); 
    printf("\n*********\nPriority\n");
}

void printMetrics(ProcessType plist[], int n)
{
    int total_wt = 0, total_tat = 0; 
    float awt, att;
    
    printf("\tProcesses\tBurst time\tWaiting time\tTurn around time\n"); 

    for (int  i=0; i<n; i++) 
    { 
        total_wt = total_wt + plist[i].wt; 
        total_tat = total_tat + plist[i].tat; 
        printf("\t%d\t\t%d\t\t%d\t\t%d\n", plist[i].pid, plist[i].bt, plist[i].wt, plist[i].tat); 
    } 
  
    awt = ((float)total_wt / (float)n);
    att = ((float)total_tat / (float)n);
    
    printf("\nAverage waiting time = %.2f", awt); 
    printf("\nAverage turn around time = %.2f\n", att); 
} 

ProcessType * initProc(char *filename, int *n) 
{
  	FILE *input_file = fopen(filename, "r");
	  if (!input_file) {
		    fprintf(stderr, "Error: Invalid filepath\n");
		    fflush(stdout);
		    exit(0);
	  }

    ProcessType *plist = parse_file(input_file, n);
  
    fclose(input_file);
  
    return plist;
}
  
// Driver code 
int main(int argc, char *argv[]) 
{ 
    int n; 
    int quantum = 2;

    ProcessType *proc_list;
  
    if (argc < 2) {
		   fprintf(stderr, "Usage: ./schedsim <input-file-path>\n");
		   fflush(stdout);
		   return 1;
	   }
    
  // FCFS
    n = 0;
    proc_list = initProc(argv[1], &n);
  
    findavgTimeFCFS(proc_list, n);
    
    printMetrics(proc_list, n);
  
  // SJF
    n = 0;
    proc_list = initProc(argv[1], &n);
   
    findavgTimeSJF(proc_list, n); 
   
    printMetrics(proc_list, n);
  
  // Priority
    n = 0; 
    proc_list = initProc(argv[1], &n);
    
    findavgTimePriority(proc_list, n); 
    
    printMetrics(proc_list, n);
    
  // RR
    n = 0;
    proc_list = initProc(argv[1], &n);
    
    findavgTimeRR(proc_list, n, quantum); 
    
    printMetrics(proc_list, n);
    
    return 0; 
} 