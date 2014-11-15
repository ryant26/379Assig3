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
}

process * create_proc_list(int number){
	return malloc(number * sizeof(process *));
}

process * init_process(int totServiceTime, int *reqResources, int numResources){
	process * proc = malloc(sizeof(process));
	proc->totalServiceTime = totServiceTime;
	proc->remainingServiceTime = 0;
	proc->resources = reqResources;
	proc->numberResources = numResources;
}

void free_all_processes(process *proc, int numberOfProc){
	//TODO
	printf("Method not implemented!\n");
}

void display_help(){
	printf("Welcome to the Bankers Algorithm Simulator!\n");
	printf("===========================================\n");
	printf("Please follow the onscreen instructions\n");
}

int get_single_number(int * buffer){
	int num;
	char term;
	if (scanf("%d%c", &num, &term) != 2 || term != '\n'){
		flush_stdin();
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
		if ((term != '\n' && term != ' ') || scanned != 2){
			flush_stdin();
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
	/* We never actually use the sames so I'm going to discard them */
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
	get_single_number(&(sim->numberOfProcesses));
}

void get_details_for_procs(simulation * sim){
	int numberResc = sim->numberResources;
	int i;
	int *resc = malloc(sizeof(int) * (numberResc +2));
	sim->processes = create_proc_list(sim->numberOfProcesses);


	for (i =0; i<sim->numberOfProcesses; i++){
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

void flush_stdin(){
	int a;
	while ((a =getchar()) != '\n'){
		if(a == EOF){
			break;
		}
	}
}