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
#define SIM_END_TIME 1000000.0
#define SEED 0
#define M 10			/* Set number of bozons */
#define S 50.0	    	/* Set mean sleep duration */
#define Y 25.0		    /* Set mean yodel duration */

/* Defining event type indicators */
#define SLEEP 1	
#define YODLING 2			    

/* TA states */
#define SILENT 0
#define MELODIOUS 1
#define SCREECH 2



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
	double melodious_start_time;
	double screech_start_time;

	/* Statistical counters for the TA */
	double total_screech_time;
	double total_melodious_time;
	double total_silent_time;

	int total_yodle;
	int total_missed_yodle;
	int attempted_yodle;
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
	double duration;

	/* ************************************************************** */

	/* *************** Initialization ******************************* */
	// You should initialize ALL VARIABLES before the main loop starts
	curr_time = 0.0;
    curr_event_index = 0;
	Reza.status = SLEEP;

	/* Statistical counters */
	Reza.total_yodle = 0;
	Reza.total_missed_yodle = 0;
	Reza.screech_start_time = 0.0;

	Reza.screech_start_time = 0.0;
	Reza.melodious_start_time = 0.0;
	Reza.total_screech_time = 0.0;
	Reza.total_melodious_time = 0.0;
	Reza.total_silent_time = 0.0;
	Reza.attempted_yodle = 0;

	// You should initialize your random number generator
	// You should generate INITIAL EVENTS before the main loop starts
	/* ************************************************************** */
	srandom(SEED);

	// Genearte the initial time of yodling of each bozons. Save it to struct with initial type of sleep.
	for (int i=0; i< M; i++) {
		double yodling_period = getExponentialVar(Y);
        //cout << yodling_period << endl;
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
			 
			 // dont have to consdier silent case only meldodiosu screehcy
			 // if you are in screech and bozons number >2 switch to meldious to  if bozons number >2 switch to screechy 
			 // screechy to schrrechy or screechy to meldious
			 
			 if (Reza.status == MELODIOUS) {
				/* Updating state variable */
				Reza.status = SCREECH;
				// SCRATCH TIME 
				Reza.screech_start_time = curr_time;
				
				/* Updating statistical counter */
				// MELDIOUS START TIME 
				Reza.total_melodious_time = Reza.total_melodious_time + (curr_time - Reza.melodious_start_time);

				/* Generating a Receive event (receiving student's next question) */
				duration = getExponentialVar(S);
				next_event.time = curr_time + duration;
				next_event.type = YODLING;
				arraysof_Events[curr_event_index] = next_event;
			 }
			 
			 else if (Reza.status == SCREECH) {
				 if (Reza.total_yodle > 2) {
					 /* Updating state variable */
					Reza.status = SCREECH;
					// SCRATCH TIME 
					Reza.screech_start_time = curr_time;

					/* Updating statistical counter */
					// TOTAL SCRATCH TIME 
					Reza.total_screech_time = Reza.total_screech_time + (curr_time - Reza.screech_start_time);

					/* Generating a Receive event (receiving student's next question) */
					duration = getExponentialVar(S);
					next_event.time = curr_time + duration;
					next_event.type = YODLING;
					arraysof_Events[curr_event_index] = next_event;	 
				 }
				 else {
					 /* Updating state variable */
					Reza.status = MELODIOUS;
					// MEDLIOUS TIME SHOULD BE 
					Reza.melodious_start_time = curr_time; 

					/* Updating statistical counter */
					// TOTAL SCRATCH TIME NEED
					Reza.total_screech_time = Reza.total_screech_time + (curr_time - Reza.screech_start_time);

					/* Generating a Receive event (receiving student's next question) */
					duration = getExponentialVar(S);
					next_event.time = curr_time + duration;
					next_event.type = YODLING;
					arraysof_Events[curr_event_index] = next_event;	
				 }
					 
			 }
			 Reza.total_yodle = Reza.total_yodle - 1;
			 Reza.attempted_yodle++;
			break;

	
		case YODLING:// When case for yodling you have to genearte sleep time by calling it getexponential function one more time
			/* You may need to do a combination of the following actions:
			 * Checking state variables
			 * Updating statistical counter
			 * Updating state variable
			 * Generating new Event(s)
			 */
			 // if sleeping - 1 yodling + 1

			// if silent not supposed to received silent event 
			if (Reza.status == SILENT) {		
				/* Updating state variable */
				Reza.status = MELODIOUS;
				// MEDLIOUS START TIME 
				Reza.melodious_start_time = curr_time;

				/* Updating statistical counter */
				Reza.total_silent_time = Reza.total_silent_time + (curr_time - Reza.melodious_start_time);

				/* Generating a sleep event  */
				duration = getExponentialVar(Y);
				next_event.time = curr_time + duration;
				next_event.type = SLEEP;
				arraysof_Events[curr_event_index] = next_event;		
			}
			
			else if (Reza.status == MELODIOUS) {
				/* Updating state variable */
				Reza.status == SCREECH;
				// SCARTCH START TIME 
				Reza.screech_start_time = curr_time;

				/* Updating statistical counter */
				// TOTAL MELDIOUS TIME 
				Reza.total_melodious_time = Reza.total_melodious_time + (curr_time - Reza.melodious_start_time);

				/* Generating a sleep event  */
				duration = getExponentialVar(Y);
				next_event.time = curr_time + duration;
				next_event.type = SLEEP;
				arraysof_Events[curr_event_index] = next_event;
			}
			else if (Reza.status == SCREECH) {
				/* Updating state variable */
				Reza.status == SCREECH;
				// TOTAL SCRATCH TIME 
				Reza.total_screech_time = Reza.total_screech_time + (curr_time - Reza.screech_start_time);

				// SCRATCH START TIME 
				Reza.screech_start_time = curr_time;

				/* Updating statistical counter */

				/* Generating a sleep event  */
				duration = getExponentialVar(Y);
				next_event.time = curr_time + duration;
				next_event.type = SLEEP;
				arraysof_Events[curr_event_index] = next_event;
			}
			Reza.total_yodle = Reza.total_yodle + 1;
			
			break;


		default:
			printf("Invalid type of event:");
		}

//#ifdef DEBUG
		//printf("End of the main loop.\n");
//#endif
	}
		// How many bozons are yodling only if 1 bozon is yodling change it from screechy to meldious
		
	// You may need to update your statistical counters one last time based on the most recent state of the system
	if (Reza.status == MELODIOUS) {
		Reza.total_melodious_time += curr_time - Reza.melodious_start_time;
	} else if (Reza.status == SCREECH) {
		Reza.total_screech_time += curr_time - Reza.screech_start_time;
	}


	/* Report */
	printf("Simulation is finished for %.0lf \n", SIM_END_TIME);
	printf("Total number of attmpted yodels = %d\n", Reza.attempted_yodle);
	printf("percentage of silent= %.2lf%c\n", 100 * Reza.total_silent_time / curr_time, '%');
	printf("percentage of melodious= %.2lf%c\n", 100 * Reza.total_melodious_time / curr_time, '%');
	printf("percentage of screechy= %.2lf%c\n", 100 * Reza.total_screech_time / curr_time, '%');
	printf("Screech time on the channel = %.2lf%c\n", 100 * Reza.total_screech_time / curr_time, '%');
	
	return 0;




	/*else if (Reza.status == IDLE) {
		Reza.total_idle_time += curr_time - Reza.idle_start_time;
	}
	*/	
	// You can report your results at the end of the program

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


