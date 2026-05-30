#include <stdio.h>
#include <gsl/gsl_rng.h>

/*Header file for the cooperation code! */

/* SET IMMUTABLE PARAMETERS */

#define TO_CONTINUE 1 /*will you possible continue this simulation? 1 if you will.*/
int CONTINUATION; /*is the current simulation a continuation of a pre-existing one? 1 if so.*/
FILE *previousdata, *lastgen_output;
int new_start_time;
int endtime;
int current_iteration;

#define BINARY 0 /* compete two strategies? 0 no, 1 yes */
#define POPULATION 1000 /* population size */
int GENERATIONS; /* for how many generations the simulation runs */
#define PHENOTYPE_DIMENSION CHANGEDIM /* number of continuous phenotypes */

/*modifications for islands */
#define ISLANDS 1
#define MIGRATION_RATE 0

#define DEFECTORS 0 /* 1= add obligate defectors */
double DEFECTOR_MUTATION;
double DEFECTOR_IC;
int defector_list[POPULATION][ISLANDS];

/* include context-free and/or context-based strategies? 1 = yes, 0 = no*/

#define CONTEXT_FREE 1
#define CONTEXT 1
#define COMPETE_TWO_SCALERS 0


#define CF_v_C 1 /*competing context free vs context dependent: 1 = 50/50, 2 = all context free, add one context after burn in, 3 = all context, add one context free after burn in */
#define CONTEXT_SWITCH 0.00

#define CONTEXT_ALONE 0/*turn this on if you wish to simulate on context individuals (on = 1)*/

/* multiplicative kernel vs. additive for mutations */

#define MULTIPLICATIVE_KERNEL false
#define ADDITIVE_KERNEL true

#define tSIGMA .5 /*scale of noncooperation of additive kernel for the threshold */
#define sSIGMA .01 /*scale of noncooperation for the scalers*/

#define ENTERPARAMETERS 1 /* set to 1 if you intend to enter parameters directly from commandline, if not, parameters should be directly entered here: */

double SELECTION_STRENGTH;
double PHENOTYPE_MUTATION;
double STRATEGY_MUTATION;
double BENEFIT;
double COST;
int ITERATION;
double STRICT_THRESH;
double GENEROUS_THRESH;
double HARDCODED_ZETA;
double STRICT_PERCENT;
double GENEROUS_PERCENT;
int SURVEY;

int endtime;

/* SET INITIAL CONDITIONS */

unsigned long int simulationseed;

//BURN_IN: time before printing values occurs
#define BURN_IN 500
#define INVASION_TIME 1000

#define STEP_SIZE 100 /* Leave at 1 to print every time step */
#define FITNESS_STEP_SIZE 100

//LOGTIME: would you like printing to be on the log scale? Then set LOGTIME = 1, otherwise set to 0.
#define LOGTIME 0 /*Set STEP_SIZE = 1 if LOGTIME = 1. */

//Set initial conditions
double INITIAL_CONTEXTFREE_THRESH;
double INITIAL_CONTEXT_SCALER;
#define INITIAL_PHENOTYPE 300.0

double INVADING_CONTEXT_SCALER;

double TEST_SCALER_IC;

//Introduce random variation around selected IC (i.e. 0.5 around 300 => IC values scattered from 150 to 450)
#define SCATTER_CONTEXTFREE_THRESH 0.0
#define SCATTER_CONTEXT_SCALER 0.0
#define SCATTER_PHENOTYPE 0.5

/* SET OUTPUT FILES */

#define stat_on true /*print summary file (saves a lot of space) */
#define fitnesses_on false /*print details of each individual %step_size time points. */
FILE *stat_output, *fitness_output;

/* add a file just for the parameters */
FILE *parameter_output;

/* the program's most-used arrays */

double phenotypes[POPULATION][PHENOTYPE_DIMENSION][ISLANDS];
double payoffs[POPULATION][ISLANDS];

double thresholds[POPULATION][ISLANDS];
double scalers[POPULATION][ISLANDS];
double acting_thresholds[POPULATION][ISLANDS];

int cf_or_c[POPULATION][ISLANDS]; /* +1 for cf and -1 for c */

int sibling_donations;
int siblings[POPULATION][ISLANDS];

/*quantify helping without mutual games */

int total_donations[ISLANDS];
int donation_events[POPULATION][ISLANDS];

double results[POPULATION][ISLANDS];

/* the program's functions */

void initialize(FILE**);

void interaction();

void new_generation(const gsl_rng* r, int time);

void append_files(int time, int current_iteration, FILE**, FILE**, FILE**, FILE**);

