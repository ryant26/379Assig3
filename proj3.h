#ifndef PROJECT_3
#define PROJECT_3

typedef enum {
	STARTED,
	FINISHED,
	ARRIVED,
	UNARRIVED,
	INVALID,
	IARRIVED
} status;

typedef struct {
	int totalServiceTime;
	int remainingServiceTime; 
	int * resources;
	int numberResources;
	status procStatus;
} process;

typedef struct{
	char ** resourceNames;
	int * resources;
	int numberResources;
	int simulationTime;
	process ** processes;
	int numberOfProcesses;
} simulation;

process ** create_proc_list(int);
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
void print_process_arrived(int);
void print_process_started(int);
void print_processed_finished(int);
void print_proccess_idle(int);
void print_process_running(int);
void print_system_status(simulation *);
void print_simulation_summary(simulation *);
void validate_processes(simulation *);
void validate_process(process *, simulation *);
void flush_stdin();
void run_simulation(simulation *);
void step_simulation(simulation *);
void check_for_arrived_proc(simulation *);
void start_processes(simulation *);
void step_processes(simulation *);
void finish_processes(simulation *);
int simulation_over(simulation *);
int processes_running(simulation *);
int no_more_processes(simulation *);
int can_start_process(simulation *, process *);
void free_resources(simulation *);

#endif