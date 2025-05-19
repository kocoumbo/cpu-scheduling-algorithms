/* This is code developed by <MARDOCHEE KABANGU KALAMBAYI> */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <bits/getopt_core.h>


struct process
{
  // data of a process
  int prc_num;
  int burst_time;
  int arrival_time;
  int priority;
  int completion_time;
  int turn_around_time;
  int waiting_time;
  int completed;
  int r_burst_time;
  struct process *next; //pointer to the next process
};


void r_file(const char *i_file, int *i,struct process **head, struct process **tail);
void output_file(const char *o_file, struct process *head, double *average_waiting_time, int num);
void process_node(int *i, struct process *prc, struct process **head, struct process **tail);
void free_linked_list(struct process *head);
void fcfs(struct process *head, int i, double *average_waiting_time);
void sjf_non_preemptive(struct process *head,int i, double *average_waiting_time);
void sjf_preemptive(struct process *head,double *average_waiting_time, int i);
void priority_non_preemptive(struct process *head, double *average_waiting_time, int i);
void priority_preemptive(struct process *head, double *average_waiting_time, int i);
void round_robin(struct process *head, double *average_waiting_time, int time_quantum, int i);
void print_output(struct process *head, double *average_waiting_time, int num);
int main(int argc, char *argv[])
{
  
    long time_quantum=0;
    const char *i_file=NULL;
    const char *o_file=NULL;
    int flag=1;
    int opt;
    
   
    
  FILE *f;
  struct process prc;
  
     //parse command line argument knowing that "t", "f", "o" have arguments
    while((opt=getopt(argc, argv, "t:f:o:" ))!=-1)
    {
      switch(opt)
      {
        case 't':{
         char *endptr;
         time_quantum=strtol(optarg,&endptr,10); //convert the string argument to a long integer
         if(*endptr != '\0')//check if the time quantum was valid
         {
          fprintf(stderr, " ERROR: the time quantum is invalid '%s'\n", optarg);
          return 1;//exit witih error
         }
        break;}
        case 'f':{ 
        i_file=optarg; //save input file name
       break;}
       case 'o':{
       o_file=optarg; //save output file
      break;}
       case '?':{
        if(optopt =='t'){
        fprintf(stderr, " ERROR: option -%c requires an argument\n",optopt); //print error 
         } else if(optopt =='f'||optopt =='o'){
          fprintf(stderr, " ERROR: option -%c requires an argument\n", optopt); //print error
         }else {
          fprintf(stderr, "ERROR: This option -%c doesn't exist\n ", optopt);
         }
        return 1; //exit with an error
      }}
    }
    //check the right value for the time quantum
    if (time_quantum<=0){
        fprintf(stderr, " ERROR: Time quantum must be an a integer greater than 0.\n ");
        return 1;
    }
    //check for missimg input and output files argument 
    if(i_file==NULL || o_file==NULL){
         fprintf(stderr, " ERROR: You must specify the input and output files.\n ");
         return 1;
    }
    int i=0;
    struct process *head=NULL;
    struct process *tail=NULL;
    double average_waiting_time=0.0;
    
    FILE *f1=fopen(o_file, "w");
    if(f1==NULL)
      fclose(f1);
    //FCFS 
    r_file(i_file,&i,&head,&tail);
    fcfs(head,i,&average_waiting_time);
    output_file(o_file,head,&average_waiting_time,1);  
    print_output(head,&average_waiting_time,1);
    free_linked_list(head);
    
    //SJF NON PREEMPTIVE
    head = tail = NULL;
    average_waiting_time = 0.0;
    i = 0;
    r_file(i_file,&i,&head,&tail);
    sjf_non_preemptive(head,i,&average_waiting_time);
    output_file(o_file,head,&average_waiting_time,2);
    print_output(head,&average_waiting_time,2);
    free_linked_list(head);

    //SJF PREEMPTIVE
    head = tail = NULL;
    average_waiting_time = 0.0;
    i = 0;
    r_file(i_file,&i,&head,&tail);
    sjf_preemptive(head,&average_waiting_time,i);
    output_file(o_file,head,&average_waiting_time,3);
    print_output(head,&average_waiting_time,3);
    free_linked_list(head);
    
    //PRIORITY NON PREEMPTIVE
    head = tail = NULL;
    average_waiting_time = 0.0;
    i = 0;
    r_file(i_file,&i,&head,&tail);
    priority_non_preemptive(head,&average_waiting_time,i);
    output_file(o_file,head,&average_waiting_time,4);
    print_output(head,&average_waiting_time,4);
    free_linked_list(head);

    //PRIORITY PREEMPTIVE
    head = tail = NULL;
    average_waiting_time = 0.0;
    i = 0;
    r_file(i_file,&i,&head,&tail);
    priority_preemptive(head,&average_waiting_time,i);
    output_file(o_file,head,&average_waiting_time,5);
    print_output(head,&average_waiting_time,5);
    free_linked_list(head);

    //ROUND ROBIN
    head = tail = NULL;
    average_waiting_time = 0.0;
    i = 0;
    r_file(i_file,&i,&head,&tail);
    round_robin(head,&average_waiting_time,time_quantum,i);
    output_file(o_file,head,&average_waiting_time,6);
    print_output(head,&average_waiting_time,6);
    free_linked_list(head);

 return 0;
     
}

//this function will help us to read data from the  input file
void r_file(const char *i_file, int *i, struct process **head, struct process **tail)
{ 
    FILE *f;
    f=fopen(i_file, "r");
    if(f==NULL){
    printf(" Can not open the file.\n ");
    return ; //an error occured 
  } 
  char *line=NULL;
  size_t length=0;
  ssize_t read;

  while((read=getline(&line, &length, f))!=-1){
  struct process prc;
  if(sscanf(line," %d:%d:%d ", &prc.burst_time, &prc.arrival_time, &prc.priority)==3){
  
  process_node(i,&prc,head,tail);
  (*i)++;
  }
  else{
    fprintf(stderr, " This format is not valid:%s\n ", line);
  }
}
free(line);
fclose(f);
}
//This function will help us to print out in the output file
void output_file(const char *o_file, struct process *head, double *average_waiting_time, int num){
  FILE *f1;
    f1=fopen(o_file, "a");
    if(f1==NULL){
      printf(" Can not open the file.\n");
      return ; //an error occured
    }
    fprintf(f1, "%d:", num);
  struct process *temp=head;
  while(temp!=NULL){
   fprintf(f1,"%d:",temp->waiting_time);
   temp=temp->next;
  }
  fprintf(f1,"%.2f\n",*average_waiting_time);
  fclose(f1);
}
void print_output(struct process *head, double *average_waiting_time, int num){
  printf("%d:", num);
  struct process *temp=head;
  while(temp!=NULL){
   printf("%d:",temp->waiting_time);
   temp=temp->next;
  }
  printf("%.2f\n",*average_waiting_time);
}
void process_node(int *i, struct process *prc, struct process **head, struct process **tail ){
 
 //create new process node

  struct process *new_process=(struct process *)malloc(sizeof(struct process));
  new_process->prc_num=(*i)+1;
  new_process->burst_time=prc->burst_time;
  new_process->arrival_time=prc->arrival_time;
  new_process->priority=prc->priority;
  new_process->completion_time=0; 
  new_process->turn_around_time=0;
  new_process->waiting_time=0;
  new_process->next=NULL;

  //add to linked list
  if(*head==NULL){
    *head=new_process;
    *tail=new_process; //if the list is empty
  }else{
    (*tail)->next=new_process; //add to the end of the list
    *tail=new_process;
  }
 
}
//free the linked list
void free_linked_list(struct process *head){
  struct process *temp_free;
  while(head!=NULL){
     temp_free=head;
     head=head->next;
     free(temp_free);
  }
}
//functio for FIRST COME FIRST SERVE scheduling
void fcfs(struct process *head, int i, double *average_waiting_time){
         
  //Let's start by sorting our linked list by arrival time
    if(head!=NULL && head->next!=NULL && i!=0){
      struct process *present_node; 
      struct process *next_node;
      int temp_prc_num;
      int temp_burst_time;
      int temp_arrival_time;

      //we're using bubble sort for sorting the linked list 
      for(present_node=head; present_node!=NULL; present_node=present_node->next){
        for(next_node=present_node->next; next_node!=NULL; next_node=next_node->next){
          if(present_node->arrival_time>next_node->arrival_time){
            temp_prc_num=present_node->prc_num;
            temp_burst_time=present_node->burst_time;
            temp_arrival_time=present_node->arrival_time;

            present_node->prc_num=next_node->prc_num;
            present_node->burst_time=next_node->burst_time;
            present_node->arrival_time=next_node->arrival_time;

            next_node->prc_num=temp_prc_num;
            next_node->burst_time=temp_burst_time;
            next_node->arrival_time=temp_arrival_time;
          }
        }
      } 
    }
    else{
      fprintf(stderr, "ERROR:  The node must not be empty ! ");
      return;
    }
    //processes in fcfs order
    int sum_waiting_time=0;
    int current_time=0;
    struct process *present_node=head;
    int prc_count=1;
    
    while(present_node!=NULL){
      //use the start time of a process as current time
     if(current_time<present_node->arrival_time){
      current_time=present_node->arrival_time;
     }
     //calculate the completion time
     present_node->completion_time=current_time+present_node->burst_time;
     //calculate the turn around time
     present_node->turn_around_time=present_node->completion_time-present_node->arrival_time;
     //calculate waiting time
     present_node->waiting_time=present_node->turn_around_time-present_node->burst_time;

     current_time=present_node->completion_time;
    //calculate the sum of waiting time
     sum_waiting_time+=present_node->waiting_time;

     
    //move to the next process
     present_node=present_node->next;

    }
    //calculate the average waiting time
    *average_waiting_time=(double)sum_waiting_time/i;
  
}
//function for SHORTEST JOB(SJF) NON PREEMPTIVE scheduling
void sjf_non_preemptive(struct process *head,int i, double *average_waiting_time){
  if (head==NULL || i <= 0) {
        fprintf(stderr, "ERROR: sjf non preemptive, the node must no be empty.\n");
        return;
    }

    struct process *current_node = NULL;
    struct process *selected_node = NULL;
    int current_time = 0;
    int completed_processes = 0;
    int  sum_waiting_time = 0;
    //main loop of the scheduling
    while (completed_processes < i) {
        current_node = head;
        selected_node = NULL;

        // Find the process with the smallest burst time that has arrived and is not completed
        while (current_node!=NULL) {
            if (current_node->arrival_time <= current_time && current_node->completed == 0) {
                if (selected_node==NULL || current_node->burst_time < selected_node->burst_time ||
                    (current_node->burst_time == selected_node->burst_time && current_node->arrival_time < selected_node->arrival_time)) {
                    selected_node = current_node;//select the rpocess with the smallest burst time
                }
            }
            current_node = current_node->next;// move to the next process
        }

        if (selected_node==NULL) {
            // No process available; increment time
            current_time++;
            continue;//move on to the next iteration
        }

        // Process the operations of the selected process
        selected_node->completion_time = current_time + selected_node->burst_time;
        selected_node->turn_around_time = selected_node->completion_time - selected_node->arrival_time;
        selected_node->waiting_time = selected_node->turn_around_time - selected_node->burst_time;

        // Update 
        current_time = selected_node->completion_time;
        selected_node->completed = 1;//mark the process as completed
        completed_processes++;
        sum_waiting_time += selected_node->waiting_time;
      
    }

    // Calculate average waiting time
    *average_waiting_time = (double)sum_waiting_time / i;
}
//function for SHORTEST JOB FIRST(SJF) PREEMPTIVE scheduling
void sjf_preemptive(struct process *head,double *average_waiting_time, int i){

   if (head==NULL || i <= 0) {
        fprintf(stderr, "ERROR: sjf preemptive, the node must not be empty.\n");
        return;
    }  

    int current_time=0;
    struct process *current_node;
    struct process *selected_node;
    int sum_burst_time=0;
    int completed_processes=0;
    int sum_waiting_time=0;
    //initialize the remaining burst time and the flag for each process
    current_node=head;
    while(current_node!=NULL){
      current_node->r_burst_time=current_node->burst_time;//set the remaining burst time
      current_node->completed=0; //flag to show that any process has not been completed 
      current_node=current_node->next;
    }
    //the main loop of scheduling
    while (completed_processes<i)
    {
      current_node=head;
      selected_node=NULL;
       int min_burst=__INT_MAX__;//initialize minimum burst time
    
         while (current_node!=NULL) {
            if (current_node->arrival_time <= current_time && current_node->completed == 0 && current_node->r_burst_time>0 && 
            current_node->r_burst_time<min_burst) {
                    min_burst=current_node->r_burst_time;//update min burst
                    selected_node = current_node;//select the current process
                }
            
            current_node = current_node->next; //move to the next process
        }
        if(selected_node!=NULL){
          selected_node->r_burst_time--;//decrement the remaining burst time
          current_time++;//increment the current time
        //process the operations of the selected pprocess  
        if(selected_node->r_burst_time==0){
        selected_node->completed = 1;//flag to show that a process has been completed
        completed_processes++;
        selected_node->completion_time=current_time;
        selected_node->turn_around_time = selected_node->completion_time - selected_node->arrival_time;
        selected_node->waiting_time = selected_node->turn_around_time - selected_node->burst_time;

        
        sum_waiting_time += selected_node->waiting_time;
        
           }
        }
        else{
          current_time++;//increment time if no process is available
        }
    }
      
      *average_waiting_time = (double)sum_waiting_time / i;
    }
    //function for PRIORITY NON PREEMPTIVE scheduling
void priority_non_preemptive(struct process *head, double *average_waiting_time, int i){
      if (head==NULL || i <= 0) {
        fprintf(stderr, "ERROR: priority non preemptive, the node must not be empty.\n");
        return;
    }

    struct process *current_node;
    struct process *selected_node = NULL;
    int current_time = 0;
    int completed_processes = 0;
    int  sum_waiting_time = 0;
     //initialize the flag for each process
     current_node=head;
    while(current_node!=NULL){
      current_node->completed=0;//flag to show that any process has not been completed
      current_node=current_node->next;
    }
    //main loop of scheduling
    while (completed_processes < i) {
        current_node = head;
        selected_node = NULL;
      

        // Find the process with the highest priority that has arrived and is not completed
        if(current_time==0){
       while (current_node!=NULL) {
            if (current_node->arrival_time <= current_time && current_node->completed == 0) {
                selected_node=current_node;//select the first process available
                break;//exit the loop
                }
                current_node=current_node->next;//move to the next process
       }
    } else{
                 int min_priority=__INT_MAX__;//initialize minimum priority
                 while (current_node!=NULL){
                   if (current_node->arrival_time <= current_time && current_node->completed == 0){
               if(current_node->priority<min_priority){
                    min_priority=current_node->priority;//update minimum priority
                    selected_node = current_node;//select the current process
                }else if(current_node->priority == min_priority && current_node->arrival_time<selected_node->arrival_time){
                      selected_node=current_node;//select based of arrival time if priorities are equal
                }
            }
            
            current_node = current_node->next;//move to the next process
                 }
        }
        //process the operations of the selected process
        if(selected_node!=NULL){
        selected_node->completion_time=current_time+selected_node->burst_time;
        selected_node->turn_around_time = selected_node->completion_time - selected_node->arrival_time;
        selected_node->waiting_time = selected_node->turn_around_time - selected_node->burst_time;

        // Update 
        selected_node->completed = 1;//flag to show that a process has been completed
        completed_processes++;
        sum_waiting_time += selected_node->waiting_time;
        current_time=selected_node->completion_time;
          
        }
        else{
          current_time++;//increment the time if no process is available
        }
    }
      
      *average_waiting_time = (double)sum_waiting_time / i;
      
    }
    //function for PRIORITY PREEMPTIVE scheduling
void priority_preemptive(struct process *head, double *average_waiting_time, int i){

  
   if (head==NULL || i <= 0) {
        fprintf(stderr, "ERROR: priority preemptive, the node must not be empty.\n");
        return;
    }  

    int current_time=0;
    struct process *current_node;
    struct process *selected_node;
    int sum_burst_time=0;
    int completed_processes=0;
    int sum_waiting_time=0;
    //initialize the remaining burst time and the flag for each process
    current_node=head;
    while(current_node!=NULL){
      current_node->r_burst_time=current_node->burst_time;//set the remaining burst time
      current_node->completed=0;//flag to show that any process has not been completed
      current_node=current_node->next;//move to the next proocess
    }
    //main loop of scheduling
    while (completed_processes<i)
    {
      current_node=head;
      selected_node=NULL;
       int min_priority=__INT_MAX__;//initialize the minimum priority
               // Find the process with the highest priority that has arrived and is not completed
                 while (current_node!=NULL){
                   if (current_node->arrival_time <= current_time && current_node->completed == 0){
               if(current_node->priority<min_priority){
                    min_priority=current_node->priority;//update minimum priority
                    selected_node = current_node;//select the current process
                }else if(current_node->priority == min_priority && current_node->arrival_time<selected_node->arrival_time){
                      selected_node=current_node;//select based of arrival time if priorities are equal
                }
            }
            
            current_node = current_node->next;//move to the next process
                 }
        if(selected_node!=NULL){
          selected_node->r_burst_time--;//decrement the remaining burst time
          current_time++;//increment the current time
         //process the operations of the selected process 
        if(selected_node->r_burst_time==0){
        selected_node->completed = 1;
        completed_processes++;
        selected_node->completion_time=current_time;
        selected_node->turn_around_time = selected_node->completion_time - selected_node->arrival_time;
        selected_node->waiting_time = selected_node->turn_around_time - selected_node->burst_time;

      
        sum_waiting_time += selected_node->waiting_time;
        
           }
        }
        else{
          current_time++;//increment current time if no process is available
        }
    }
      
      *average_waiting_time = (double)sum_waiting_time / i;
}
//function of ROUND ROBIN scheduling
void round_robin(struct process *head, double *average_waiting_time, int time_quantum, int i){
   if (head==NULL || i <= 0) {
        fprintf(stderr, "ERROR: round robin, the node must not be empty.\n");
        return;
    }  

    int current_time=0;
    struct process *current_node;
    int sum_waiting_time=0;
    int completed=0;//flag to show that any process has not been completed
    int completed_processes=0;
  
    //initialize the remaining burst time and the waiting time of each process
    current_node=head;
    while(current_node!=NULL){
      current_node->r_burst_time=current_node->burst_time;//set remaining burst time
      current_node->waiting_time=0;//initialize waiting time
      current_node=current_node->next;
    }
    //main loop for scheduling
    while(completed_processes<i){
      completed=1;//assume all processes are completed
      current_node=head;
      //iterate through all processes
      while(current_node!=NULL){
        if(current_node->r_burst_time>0){
          completed=0;//At least one process is not completed
          if(current_node->r_burst_time>time_quantum){
            current_node->waiting_time+=current_time-current_node->arrival_time;
            current_time+=time_quantum;//increment current time by time quantum
            current_node->r_burst_time-=time_quantum;//decrement remaining burst time
            current_node->arrival_time=current_time;//update arrival time
          }else{
            current_node->waiting_time+=current_time-current_node->arrival_time;
            current_time+=current_node->r_burst_time;//increment burst time
            current_node->r_burst_time=0;// a process is completed
            completed_processes++;
          }
        }
        current_node=current_node->next;//move to the next proocess
      }
      if(completed){
        break;//exit the loop if all processes are completed
      }
    }
    //process the operations of the current node
    current_node=head;
    while(current_node!=NULL){
      sum_waiting_time+=current_node->waiting_time;
      
      
      current_node=current_node->next;//move to thhe next process
    }
    *average_waiting_time=(double)sum_waiting_time/i;
    
}