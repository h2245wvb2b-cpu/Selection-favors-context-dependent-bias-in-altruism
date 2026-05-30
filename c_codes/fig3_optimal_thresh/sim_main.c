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
		CONTINUATION = atof(argv[i]);i++;
		if(CONTINUATION == 1)
		{
			GENERATIONS = atof(argv[i]);i++;
		} else
		{
			GENERATIONS = endtime;
		}

		if(BINARY == 1 && CONTEXT_FREE == 1)
		{	
			STRICT_THRESH = atof(argv[i]);i++;
			GENEROUS_THRESH = atof(argv[i]);i++;
		} else if(BINARY == 0 && CONTEXT_FREE == 1)
		{
			INITIAL_CONTEXTFREE_THRESH = atof(argv[i]);i++;
		}
		if(CONTEXT == 1)
		{
			INITIAL_CONTEXT_SCALER = atof(argv[i]);i++;
			if(CONTINUATION == 1)
			{	
				TEST_SCALER_IC = atof(argv[i]);i++;
			}
		} else if(CONTEXT == 0 && CONTEXT_FREE == 0)
		{
			exit(0);
		}

		if(DEFECTORS == 1)
		{
			DEFECTOR_MUTATION = atof(argv[i]);i++;
			DEFECTOR_IC = atof(argv[i]);i++;
		}
		if(COMPETE_TWO_SCALERS == 1)
		{
			INVADING_CONTEXT_SCALER = atof(argv[i]);i++;
		}

		ITERATION = atoi(argv[i]);i++;

	}

	current_iteration = ITERATION;

	initialize(&previousdata);

	double sample_times[(int) (log((endtime - 1000))/log(10)/0.01)];
	
	if(LOGTIME == 1)
	{

		for(int i = 0; i < (int) (log((endtime - 1000))/log(10)/0.01); i++)
		{
			sample_times[i] = pow(10, i*0.01);

		}
	} else
	{
		for(int i = 0; i < (int) (log((endtime - 1000))/log(10)/0.01); i++)
		{
			sample_times[i] = -1;
		}
	}
	
	int time_index = 0;
	
	int preferred_time = sample_times[time_index];

/*addition for checking the effects of invading with different initial conditions */

	if(CONTINUATION == 1)
	{
		for(int i = 0; i < POPULATION; i++)
		{
			for(int island = 0; island < ISLANDS; island++)
			{	
				scalers[i][island] = TEST_SCALER_IC;

				if(SCATTER_CONTEXT_SCALER != 0)
				{
					if(i < POPULATION/2)
					{
						scalers[i][island] *= (1 + (double)rand()/RAND_MAX*SCATTER_CONTEXT_SCALER);
					} else
					{
						scalers[i][island] *= (1 - (double)rand()/RAND_MAX*SCATTER_CONTEXT_SCALER);
					}
				}
			}
		}
	}

	for (int time = 0; time <= endtime; time++)
	{

		if(LOGTIME == 1)
		{
			if(time > preferred_time && preferred_time != (int) sample_times[ (int) (log((GENERATIONS - 1000))/log(10)/0.01) - 1])
			{
				while(time > preferred_time)
				{
					time_index++;
					preferred_time = sample_times[time_index];
				}
			}
		
			if(time == 0 || time == (int) preferred_time){
		
				append_files(time, current_iteration, &stat_output, &fitness_output, &parameter_output, &lastgen_output);
		
				if( preferred_time != (int) sample_times[ (int) (log((GENERATIONS - 1000))/log(10)/0.01) - 1])
				{
					time_index++;
				}
				preferred_time = sample_times[time_index];
			};
		} else
		{
			append_files(time, current_iteration, &stat_output, &fitness_output, &parameter_output, &lastgen_output);
		}

		if(CONTINUATION == 1 && time == 0)
                {
                        time = new_start_time;
                }
	
if(time != -1){		interaction(time);};

		new_generation(r, time);
	}

	gsl_rng_free (r);

}




