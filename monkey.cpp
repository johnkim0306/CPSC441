/* -------------------------------------------------------------------------
 * This program provides a basic structure for a discrete-event simulator.
 *
 * Name              : start_code.c
 * Author            : Reza Gholizadeh
 * Language          : C
 * Latest Revision   : 11-20-2018
 * -------------------------------------------------------------------------
 * To compile gcc -o start_code start_code.c -lm 
 * To run ./start_code
 */

/* -------------- Libraries ------------------------ */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* -------------- Global variables ------------------------ */
#define MAX_INT 2147483647 /* Maximum positive integer 2^31 - 1 */
#define SIM_END_TIME 100000.0
#define SEED 0
#define M 100			/* Set number of bozons */
#define S 50.0	    	/* Set mean sleep duration */
#define Y 25.0		    /* Set mean yodel duration */

/* Defining event type indicators */
#define SLEEP 1				    // This is for melodios time
#define YODLING 2			    // THis is for screechy time

/* Debugging flag */
#define DEBUG 1

using namespace std;


/* -------------- Structures ------------------------------ */
// Events can be defined as structures. Each event has at least two elements: 1.time 2.type
struct Event {
	double time;
	int type;
};

struct TA {
	/* State variables */
	int status; /* Represents the status of the TA. It can be either IDLE or BUSY */    
    double sleep_start_time;
    double yodle_start_time;

	/* Statistical counters for the TA */
	double total_sleep_time;
	double total_yodle_time;
	int total_yodle;
	int total_missed_yodle;
};


// You can define your program entities (e.g. Bozon, Atmosphere and etc.) as structures.

/* -------------- Function declarations ------------------- */
double uniform01(void);
double getExponentialVar(double mean);

/* -------------- Function definitions ------------------- */
/* ============== */
int main(void)
/* ============== */
{
	/* *************** Defining Variables *************************** */
	// You should define simulator variables (e.g. time, event list, entities, statistical counters and control variables)
	double curr_time;
    int curr_event_index;                  /* Shows the index of the current event in the event_list */
	struct Event curr_event;
	struct Event next_event;				/* A dummy variable for creating next event in each iteration */
    struct Event arraysof_Events[M];       /*now put that into an arrya that has a type event */
	struct TA Reza;
	double next_yodle_start_time;
	double next_sleep_start_time;

	/* ************************************************************** */

	/* *************** Initialization ******************************* */
	// You should initialize ALL VARIABLES before the main loop starts
	curr_time = 0.0;
    curr_event_index = 0;
	Reza.status = SLEEP;
	Reza.sleep_start_time = 0.0;
	Reza.yodle_start_time = 0.0;
	/* Statistical counters */
	Reza.total_sleep_time = 0.0;
	Reza.total_yodle_time = 0.0;
	Reza.total_yodle = 0;
	Reza.total_missed_yodle = 0;

	// You should initialize your random number generator
	// You should generate INITIAL EVENTS before the main loop starts
	/* ************************************************************** */
	srandom(SEED);

	// Genearte the initial time of yodling of each bozons. Save it to struct with initial type of sleep.
	for (int i=0; i<= M; i++) {
		double yodling_period = getExponentialVar(Y);
        cout << yodling_period << endl;
        arraysof_Events[i].time = yodling_period;
        arraysof_Events[i].type = SLEEP;
	}

	/* Main loop */
	while (curr_time < SIM_END_TIME) {
		// You should find the most imminent event in the event list and store it's index in curr_event_index for execution
        double min = SIM_END_TIME;
		for (int i = 0; i < M; i++) {
			if (arraysof_Events[i].time < min) {
				min = arraysof_Events[i].time;
				curr_event_index = i;
			}
		}

		// You should update the time variable to the current time (time of the current event you want to execute) of the system
        curr_event = arraysof_Events[curr_event_index];
		curr_time = curr_event.time;

		switch (curr_event.type) {
		case SLEEP:
			/* You may need to do a combination of the following actions:
			 * Checking state variables
			 * Updating statistical counter
			 * Updating state variable
			 * Generating new Event(s)
			 */

            /* Updating state variable */
			Reza.status = YODLING;
			Reza.yodle_start_time = curr_time;

			/* Updating statistical counter */
			Reza.total_yodle_time = Reza.total_yodle_time + (curr_time - Reza.sleep_start_time)
			//Reza.total_yodle_time += curr_time - Reza.yodle_start_time;

			/* Generating a Receive event (receiving student's next question) */
			next_yodle_start_time = getExponentialVar(Y);
			next_event.time = curr_time + next_yodle_start_time;
			next_event.type = YODLING;
			arraysof_Events[curr_event_index] = next_event;
			cout << "sleeping: " << curr_time << endl;
			break;

	
		case YODLING:// When case for yodling you have to genearte sleep time by calling it getexponential function one more time
			/* You may need to do a combination of the following actions:
			 * Checking state variables
			 * Updating statistical counter
			 * Updating state variable
			 * Generating new Event(s)
			 */

			/* Updating state variable */
			Reza.status = SLEEP;
			Reza.sleep_start_time = curr_time;

			/* Updating statistical counter */
			Reza.total_yodle_time = Reza.total_yodle_time + (curr_time - Reza.yodle_start_time)
			//Reza.total_sleep_time += curr_time - Reza.sleep_start_time;

			/* Generating a sleep event  */
			next_sleep_start_time = getExponentialVar(M);
			next_event.time = curr_time + next_sleep_start_time;
			next_event.type = SLEEP;
			arraysof_Events[curr_event_index] = next_event;
			cout << "yodling: " << curr_time << endl;
			break;


		default:
			printf("Invalid type of event:");
		}

#ifdef DEBUG
		printf("End of the main loop.\n");
#endif
	}

	// You may need to update your statistical counters one last time based on the most recent state of the system
	// You can report your results at the end of the program

	printf("Simulation is finished for %.0lf days.\n", SIM_END_TIME);
	//printf("Total number of received e-mails = %d\n", Reza.total_received_emails);



}

/* Generate a random floating point number uniformly distributed in [0,1] */
/* ============== */
double uniform01()
/* ============== */
{
	double randnum;

	/* Get a random positive integer from random() */
	randnum = (double) 1.0 * random();

	/* Divide by max integer to get something in the range 0.0 to 1.0 */
	randnum = randnum / (1.0 * MAX_INT);
	return randnum;
}

/* Generate a random floating point number exponentially distributed with mean = m */
/* ============== */
double getExponentialVar(double m)    //starting time of sleeping and starting time of yolding 
/* ============== */
{
	double randnum, ans;
	randnum = uniform01();
	ans = -(m) * log(randnum);
	return ans;
}


// case sleep change it to yodle case yodle change it sleep
// beginning of the lsep - beginning of the yodle 

// If Yolding call an exponential function again to generate 