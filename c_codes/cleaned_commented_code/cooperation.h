#include <stdio.h>
#include <gsl/gsl_rng.h>

/*Header file for the cooperation code! */

/* SET IMMUTABLE PARAMETERS */

#define ENTERPARAMETERS 1 /* set to 1 if you intend to enter parameters directly from commandline, if not, parameters should be directly entered here: */
int endtime; /* for how many generations the simulation runs */
int current_iteration;
double SELECTION_STRENGTH;
double PHENOTYPE_MUTATION;
double STRATEGY_MUTATION;
double BENEFIT;
double COST;
int ITERATION;
int SURVEY;

#define POPULATION 1000 /* population size */
#define PHENOTYPE_DIMENSION CHANGEDIM /* number of continuous phenotypes */
#define CONTEXT_SWITCH 0.001

/* set to 1 if you would like the theoretically optimum thresholds, and 0 if not */
#define EMPIRICAL_THRESH 1

/* SET INITIAL CONDITIONS */

unsigned long int simulationseed;

//BURN_IN: time before printing values occurs
#define BURN_IN 500
#define INVASION_TIME 10000

#define STEP_SIZE 100 /* Leave at 1 to print every time step */
#define FITNESS_STEP_SIZE 1

//Set initial conditions
double INITIAL_CONTEXTFREE_THRESH;
double INITIAL_CONTEXT_SCALER;
#define INITIAL_PHENOTYPE 300.0

#define MAX_ALLOWED_THRESHOLD_COEFFICIENT 2.0;
#define MAX_ALLOWED_THRESHOLD 25.0;

//Introduce random variation around selected IC (i.e. 0.5 around 300 => IC values scattered from 150 to 450)
#define SCATTER_CONTEXTFREE_THRESH 0.0
#define SCATTER_CONTEXT_SCALER 0.0
#define SCATTER_PHENOTYPE 0.5

/* SET OUTPUT FILES */

#define stat_on true /*print summary file (saves a lot of space) */
#define fitnesses_on true /*print details of each individual %step_size time points. */
FILE *stat_output, *fitness_output;

/* add a file just for the parameters */
FILE *parameter_output;

/* the program's most-used arrays */

double phenotypes[POPULATION][PHENOTYPE_DIMENSION];
double payoffs[POPULATION];
int store[POPULATION][10];

double thresholds[POPULATION];
double scalers[POPULATION];
double acting_thresholds[POPULATION];

int cf_or_c[POPULATION]; /* +1 for cf and -1 for c */

/*quantify helping without mutual games */

int total_donations;
int donation_events[POPULATION];

double average_distances[POPULATION];

double results[POPULATION];

/* the program's functions */

void initialize();

void interaction(int time);

void hypothetical_optimum(int time);

void new_generation(const gsl_rng* r, int time);

void append_files(int time, int current_iteration, FILE**, FILE**);
