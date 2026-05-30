#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_sf_gamma.h>
#include <gsl/gsl_statistics_double.h>
#include "cooperation.h"

void new_generation(const gsl_rng *r, int time)
{

/*Now we repopulate for the next generation, weighted by the success in the previous games.*/

		double fitness_weights[POPULATION];

	        for (int i = 0; i < POPULATION; i++)
	        {

	                fitness_weights[i] = 1 + (results[i])*SELECTION_STRENGTH;

	        }
/* store the previous values to form the new values. */
	        double shadow_phenotypes[POPULATION][PHENOTYPE_DIMENSION];

		double shadow_thresholds[POPULATION];

		double shadow_scalers[POPULATION];

		double shadow_cf_or_c[POPULATION];

		double shadow_acting_thresholds[POPULATION];

	        for (int i = 0; i < POPULATION; i++)
	        {

	                for (int j = 0; j < PHENOTYPE_DIMENSION; j++)
	                {

	                        shadow_phenotypes[i][j] = phenotypes[i][j];

	                }

			shadow_thresholds[i] = thresholds[i];
			shadow_scalers[i] = scalers[i];
	        	shadow_cf_or_c[i] = cf_or_c[i];
			shadow_acting_thresholds[i] = acting_thresholds[i];
		}

/*Use the multinomial draw from GSL */

	        int K = POPULATION; /* # "categories" */
	        unsigned int n[POPULATION]; /*array to return results of draw */

	        gsl_ran_multinomial(r, K, POPULATION, fitness_weights, n);

	        int counter = 0;

/*Here we fill the array with all the individuals listed in n[] */

	        for (int i = 0; i < POPULATION; i++)
	        {

	                for(int j = 0; j < n[i]; j++)
	                {

	                        for (int k = 0; k < PHENOTYPE_DIMENSION; k++)
	                        {
	                                phenotypes[j+counter][k] = shadow_phenotypes[i][k];
	                        }

				thresholds[j+counter] = shadow_thresholds[i];

				scalers[j+counter] = shadow_scalers[i];

				cf_or_c[j+counter] = shadow_cf_or_c[i];

				acting_thresholds[j+counter] = shadow_acting_thresholds[i];

	                }

	                counter += n[i];

	        }

/*add a phenotype mutation */

	        for (int i = 0; i < POPULATION; i++)
	        {

	                for(int j = 0; j < PHENOTYPE_DIMENSION; j++)
	                {

	                        phenotypes[i][j] += gsl_ran_gaussian(r, PHENOTYPE_MUTATION);

	                }

	        }



/* If time is greater than the burn in, we allow strategy mutation.*/

	        if (time > BURN_IN)
	        {
	                for (int i = 0; i < POPULATION; i++)
	                {
/* mutation to strategy type. */
				if(gsl_ran_bernoulli(r, CONTEXT_SWITCH) == 1 && INVASION_TIME <= time)
				{
			
						int invader = rand()%POPULATION;
						cf_or_c[invader] *= -1;						
					
				}
						
/* mutation to the threshold value or threshold coefficient value. */
	                        if(gsl_ran_bernoulli(r, STRATEGY_MUTATION) == 1)
	                        {

						if(rand()%2 == 0)
                                                {       

						double n1 = (double)rand() / RAND_MAX;
						scalers[i] = n1*MAX_ALLOWED_THRESHOLD_COEFFICIENT;



						} else
                                                {

						double n2 = (double)rand() / RAND_MAX;
                                                thresholds[i] = n2*MAX_ALLOWED_THRESHOLD;

                                                }

                        	}

                	}

		}
}
