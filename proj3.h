#ifndef PROJECT_3
#define PROJECT_3

typedef enum {
	STARTED,
	FINISHED,
	ARRIVED,
	UNARRIVED,
	INVALID
} status;

typedef struct {
	int totalServiceTime;
	int remainingServiceTime; 
	int * resources;
	int numberResources;
	status procStatus;;
} process;

typedef struct{
	char ** resourceNames;
	int * resources;
	int numberResources;
	int simulationTime;
	process ** processes;
	int numberOfProcesses;
} simulation;

process * create_proc_list(int);
process * init_process(int, int *, int);
void free_all_processes(process *, int);
void display_help();
int get_single_number(int *);
int get_list_of_numbers(int *, int);
void get_number_of_resources(simulation *);
void get_name_of_resources(int, simulation*);
void get_number_of_each_resource(simulation *);
void get_number_of_processes(simulation *);
void get_details_for_procs(simulation *); 
void print_simulation_time(simulation *);
void print_process_statuses(simulation *);
void print_process_status(process *);
int validate_processes(simulation *);
int validate_process(process *);
void flush_stdin();

#endif