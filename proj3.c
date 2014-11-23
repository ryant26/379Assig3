#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "proj3.h"

static simulation sim;


int main (){
	display_help();
	get_number_of_resources(&sim);
	get_name_of_resources(sim.numberResources, &sim);
	get_number_of_each_resource(&sim);
	get_number_of_processes(&sim);
	get_details_for_procs(&sim);
	validate_processes(&sim);
	run_simulation(&sim);
	free_resources(&sim);
	return 0;
}

process ** create_proc_list(int number){
	return malloc(number * sizeof(process *));
}

process * init_process(int totServiceTime, int *reqResources, int numResources){
	process * proc = malloc(sizeof(process));
	proc->totalServiceTime = totServiceTime;
	proc->remainingServiceTime = totServiceTime;
	proc->resources = reqResources;
	proc->numberResources = numResources;
	return proc;
}

void display_help(){
	printf("Welcome to the Bankers Algorithm Simulator!\n");
	printf("===========================================\n");
	printf("Please follow the onscreen instructions\n");
}

int get_single_number(int * buffer){
	int num;
	char term;
	if (scanf("%d%c", &num, &term) != 2 || term != '\n' || num < 0){
		if (term != '\n'){
			flush_stdin();
		}
		return 0;
	} else {
		*buffer = num;
		return 1;
	}
}

int get_list_of_numbers(int *out, int num){
	int tok;
	int scanned;
	char term;
	int i;
	for (i = 0; i < num; i++){
		scanned = scanf("%d%c", &tok, &term);
		if ((term != '\n' && term != ' ') || scanned != 2 || tok < 0
			|| ( i+1 == num && term != '\n')){
			if (term != '\n') flush_stdin();
			return 0;
		}

		out[i] = tok;
	}
	return 1;
}

void get_number_of_resources(simulation *sim){
	int resc;
	while (1){
		printf("Please enter the number of resources that exist:\n");
		if (!get_single_number(&resc)){
			printf("invalid input, try again.\n");
			continue;
		}
		break;
	}
	sim->numberResources = resc;
}

void get_name_of_resources(int num, simulation *sim){
	/* We never actually use the names so I'm going to discard them */
	printf("Please enter the names of the resources separated by spaces: \n");
	flush_stdin();
}

void get_number_of_each_resource(simulation *sim){
	sim->resources = malloc(sim->numberResources * sizeof(int));
	printf("Please enter the number of each resource separated by spaces:\n");
	while (1){
		if (!get_list_of_numbers(sim->resources, sim->numberResources)){
			printf("invalid, please try again:\n");
			continue;
		}
		break;
	}
	
}

void get_number_of_processes(simulation *sim){
	printf("Please input the number of processes\n");
	while (1){
		if (!get_single_number(&(sim->numberOfProcesses))){
			printf("invalid, please try again:\n");
			continue;
		}
		break;
	}	
}

void get_details_for_procs(simulation * sim){
	int numberResc = sim->numberResources;
	int i;
	int *resc;
	sim->processes = create_proc_list(sim->numberOfProcesses);


	for (i =0; i<sim->numberOfProcesses; i++){
		resc = malloc(sizeof(int) * (numberResc +2));
		printf("Please enter number of resources needed in the form\n");
		printf("<A> <B> ... <N> <ArrivalTime> <ServiceTime> for P%d:\n", i);
		while (1){
			if (!get_list_of_numbers(resc, sim->numberResources + 2)){
				printf("invalid, please try again\n");
				continue;
			}
			break;
		}
		(sim->processes)[i] = init_process(resc[numberResc + 1], resc, numberResc + 2);
	}
}

void print_simulation_time(simulation * sim){
	static int lastTime = -1;
	if (sim->simulationTime != lastTime){
		printf("Simulation time: %d\n", sim->simulationTime);
		lastTime = sim->simulationTime;

		print_system_status(sim);
	}
}

void print_process_arrived(int num){
	printf("Process P%d has just arrived in the system.\n", num);
}

void print_process_started(int num){
	printf("Process P%d has just started execution.\n", num);
}

void print_process_finished(int num){
	printf("Process P%d has just finished.\n", num);
}

void print_process_idle(int num){
	printf("Process P%d is idle.\n", num);
}

void print_process_running(int num){
	printf("Process P%d is running.\n", num);
}

void print_simulation_summary(simulation *sim){
	int i;
	printf("\n");
	printf("\n");
	for (i = 0; i <  sim->numberOfProcesses; i++){
		process * currProc = sim->processes[i];
		if (currProc->procStatus == INVALID || currProc->procStatus == IARRIVED){
			printf("No processes are running.\n");
			printf("Available resources are insufficient to run\n");
			printf("any idle process. System is halted.\n");
			printf("Simulation ended.\n");
			return;
		}
	}

	printf("No processes are available for execution\n");
	printf("System is idle.\n");
	printf("Simulation ended\n");
}

void print_system_status(simulation * sim){
	int i;
	for (i = 0; i < sim->numberOfProcesses; i++){
		process * currProc = sim->processes[i];
		if ((currProc->procStatus == ARRIVED || currProc->procStatus == IARRIVED)
			&& sim->simulationTime > (currProc->resources)[currProc->numberResources - 2]){
			print_process_idle(i);
		} else if (currProc ->procStatus == STARTED 
			&& currProc->remainingServiceTime < currProc->totalServiceTime){
			print_process_running(i);
		}
	}
}

void validate_processes(simulation *sim){
	int i;
	for (i =0; i < sim->numberOfProcesses; i++){
		validate_process((sim->processes)[i], sim);
	}
}

void validate_process(process * proc, simulation * sim){
	int i;

	if (proc->totalServiceTime < 0){
		proc->procStatus = INVALID;
		return;
	}

	for (i = 0; i < sim->numberResources; i++){
		if (proc->resources[i] > sim->resources[i]){
			proc->procStatus = INVALID;
			return;
		}
	}
	proc->procStatus = UNARRIVED;
}

void flush_stdin(){
	int a;
	while ((a =getchar()) != EOF && a !='\n' );
}

void run_simulation(simulation * sim){
	while(!simulation_over(sim)){
		step_simulation(sim);
	}
	print_simulation_summary(sim);
}

void step_simulation(simulation * sim){
	finish_processes(sim);
	check_for_arrived_proc(sim);
	start_processes(sim);
	step_processes(sim);
	sim->simulationTime++;
}

int simulation_over(simulation * sim){
	static int j = 0;
	j++;
	if (!processes_running(sim) && no_more_processes(sim)){
		return 1;
	} 
	return 0;
}

int processes_running(simulation *sim){
	int i;
	for (i = 0; i < sim->numberOfProcesses; i++){
		if (((sim->processes)[i])->procStatus == STARTED){
			return 1;
		}
	}
	return 0;
}

int no_more_processes(simulation *sim){
	int i;
	for (i = 0; i < sim->numberOfProcesses; i++){
		status procStat = ((sim->processes)[i])->procStatus;
		if (procStat != INVALID && procStat != FINISHED && procStat != IARRIVED){
			return 0;
		}
	}
	return 1;
}

int can_start_process(simulation *sim, process *proc){
	int i;
	if (proc->procStatus == ARRIVED){
		for (i = 0; i < sim->numberResources; i++){
			if ((proc->resources)[i] > (sim->resources)[i]){
				return 0;
			}
		}
		return 1;
	}
	return 0;
}

void check_for_arrived_proc(simulation *sim){
	int i;
	for (i = 0; i < sim->numberOfProcesses; i ++){
		process * currProc = sim->processes[i];
		if (currProc->procStatus == UNARRIVED || currProc->procStatus == INVALID){
			if (sim->simulationTime >= (currProc->resources[sim->numberResources])){
				if (currProc->procStatus != INVALID){
					currProc->procStatus = ARRIVED;
				} else {
					currProc->procStatus = IARRIVED;
				}
				print_simulation_time(sim);
				print_process_arrived(i);
			}
		}
	}
}

void start_processes(simulation *sim){
	int i;
	for (i = 0; i < sim->numberOfProcesses; i ++){
		process * currProc = sim->processes[i];
		if (can_start_process(sim, currProc)){
			currProc->procStatus = STARTED;
			print_simulation_time(sim);
			print_process_started(i);
			int j;
			for (j = 0; j < sim->numberResources; j++){
				(sim->resources)[j] -= (currProc->resources)[j];
			}
		}
	}
}

void step_processes(simulation *sim){
	int i;
	for (i = 0; i <sim->numberOfProcesses; i++){
		process * currProc = sim->processes[i];
		if (currProc->procStatus == STARTED){
			currProc->remainingServiceTime--;
		}
	}
}
 	
void finish_processes(simulation *sim){
	int i;
	for (i =0; i < sim->numberOfProcesses; i++){
		process * currProc = sim->processes[i];
		if (currProc->procStatus == STARTED && currProc-> remainingServiceTime <= 0){
			currProc->procStatus = FINISHED;
			print_simulation_time(sim);
			print_process_finished(i);
			int j;
			for (j = 0; j < sim->numberResources; j++){
				(sim->resources)[j] += (currProc->resources)[j];
			}
		}
	}
}

void free_resources(simulation *sim){
	int i;
	for (i = 0; i < sim->numberOfProcesses; i++){
		process * currProc = sim->processes[i];
		free(currProc->resources);
		free(currProc);
	}
	free(sim->resources);
	free(sim->processes);
}