#include "cooperation.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_sf_gamma.h>
#include <gsl/gsl_statistics_double.h>

#include <unistd.h>
#include <sys/syscall.h>

#include <gsl/gsl_sort.h>
#include <gsl/gsl_sort_vector.h>


int main(int argc, char *argv[])
{
/*set up for random variable */

	const gsl_rng_type *T;
	gsl_rng * r;
	gsl_rng_env_setup();

	int current_id = getpid();

        simulationseed = gsl_rng_default_seed;


	gsl_rng_default_seed = time(NULL) + current_id;
	gsl_rng_default = gsl_rng_mt19937;
	T = gsl_rng_default;
	r = gsl_rng_alloc (T);

	simulationseed = gsl_rng_default_seed;

/* set simulation parameters.*/

	SURVEY = 10;

	if(ENTERPARAMETERS == 1)
	{

		int i = 1;
		endtime = atof(argv[i]);i++;
		SELECTION_STRENGTH = atof(argv[i]);i++;
		PHENOTYPE_MUTATION = atof(argv[i]);i++;
		STRATEGY_MUTATION = atof(argv[i]);i++;
		BENEFIT = atof(argv[i]);i++;
		COST = atof(argv[i]);i++;
		INITIAL_CONTEXTFREE_THRESH = atof(argv[i]);i++;
		INITIAL_CONTEXT_SCALER = atof(argv[i]);i++;
		ITERATION = atoi(argv[i]);i++;

	}

	current_iteration = ITERATION;

/*set initial conditions. */

	initialize();

	for (int time = 0; time <= endtime; time++)
	{

/* create output files, record values. */

			append_files(time, current_iteration, &stat_output, &fitness_output);

/*interact individuals. */

if(time != -1){		interaction(time);};

/* create next generation of individuals. */

		new_generation(r, time);
	}

	gsl_rng_free (r);

}




