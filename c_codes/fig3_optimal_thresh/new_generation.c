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

	for (int island = 0; island < ISLANDS; island++)
	{

		double fitness_weights[POPULATION];

	        for (int i = 0; i < POPULATION; i++)
	        {

	                fitness_weights[i] = 1 + (results[i][island])*SELECTION_STRENGTH;

	        }

	        double shadow_phenotypes[POPULATION][PHENOTYPE_DIMENSION];

		double shadow_thresholds[POPULATION];

		double shadow_scalers[POPULATION];

		double shadow_defector_list[POPULATION];

		double shadow_cf_or_c[POPULATION];

		double shadow_acting_thresholds[POPULATION];

		double shadow_hamilton_distances[POPULATION];

	        for (int i = 0; i < POPULATION; i++)
	        {

	                for (int j = 0; j < PHENOTYPE_DIMENSION; j++)
	                {

	                        shadow_phenotypes[i][j] = phenotypes[i][j][island];

	                }

			shadow_thresholds[i] = thresholds[i][island];
			shadow_scalers[i] = scalers[i][island];
			shadow_defector_list[i] = defector_list[i][island];
	        	shadow_cf_or_c[i] = cf_or_c[i][island];
			shadow_acting_thresholds[i] = acting_thresholds[i][island];
			shadow_hamilton_distances[i] = hamilton_distances[i][island];
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
	                                phenotypes[j+counter][k][island] = shadow_phenotypes[i][k];
	                        }

				thresholds[j+counter][island] = shadow_thresholds[i];

				scalers[j+counter][island] = shadow_scalers[i];

				defector_list[j+counter][island] = shadow_defector_list[i];

				cf_or_c[j+counter][island] = shadow_cf_or_c[i];

				acting_thresholds[j+counter][island] = shadow_acting_thresholds[i];

				hamilton_distances[j+counter][island] = shadow_hamilton_distances[i];

	                }

	                counter += n[i];

	        }

/*add a phenotype mutation */

	        for (int i = 0; i < POPULATION; i++)
	        {

	                for(int j = 0; j < PHENOTYPE_DIMENSION; j++)
	                {

	                        phenotypes[i][j][island] += gsl_ran_gaussian(r, PHENOTYPE_MUTATION);

	                }

	        }



/* If time is greater than the burn in, we allow strategy mutation. This is the "invasion" aspect.*/

		if(time == BURN_IN)
	        {

	                if(CF_v_C == 2)
	                {

        	                int invader = rand()%POPULATION;

        	                thresholds[invader][island] = -1;
                        	scalers[invader][island] = INITIAL_CONTEXT_SCALER;
                	} if(CF_v_C == 3 && CONTEXT_ALONE != 1)
                	{
                	        int invader = rand()%POPULATION;

                	        scalers[invader][island] = -1;
                	        thresholds[invader][island] = INITIAL_CONTEXTFREE_THRESH;
                	}
        	}


	        if (time > BURN_IN)
	        {
	                for (int i = 0; i < POPULATION; i++)
	                {

				if(DEFECTORS == 1)
				{
					if(gsl_ran_bernoulli(r, DEFECTOR_MUTATION) == 1)
					{
						defector_list[i][island] *= -1;
					}
				
				}

				if(gsl_ran_bernoulli(r, CONTEXT_SWITCH) == 1 && INVASION_TIME <= time)
				{
			
					if(CF_v_C == 1)
					{
						
						int invader = rand()%POPULATION;
						cf_or_c[invader][island] *= -1;						
					
					}
				}
						

	                        if(gsl_ran_bernoulli(r, STRATEGY_MUTATION) == 1/* && COMPETE_TWO_SCALERS == 1*/)
	                        {

					if(CF_v_C == 1 && COMPETE_TWO_SCALERS == 0)
					{

						if(rand()%2 == 0)
                                                {       

						double n1 = (double)rand() / RAND_MAX;
						scalers[i][island] = n1*MAX_ALLOWED_THRESHOLD_COEFFICIENT;



						} else
                                                {

						double n2 = (double)rand() / RAND_MAX;
                                                thresholds[i][island] = n2*MAX_ALLOWED_THRESHOLD;

                                                }


					}

					if(CF_v_C == 2)
					{}

					if(scalers[i][island] != -1)
					{
						if(CF_v_C == 3)
						{
							if(ADDITIVE_KERNEL == true)
                                        		{
                                        			double output = 0;
                                        			int found = 0;

                                                		while(found == 0)
                                                		{

                                                        		output = gsl_ran_gaussian(r, sSIGMA);

                                                        		if(output > (-1)*scalers[i][island])
                                                        		{

                                                                		found = 1;

                                                        		}

                                                		}

                                                		scalers[i][island] += output;
                                        		}
						}
					}
					if(BINARY == 1)
					{

						if(thresholds[i][island] == STRICT_THRESH)
						{
							thresholds[i][island] = GENEROUS_THRESH;
						} else
						{
							thresholds[i][island] = STRICT_THRESH;
						}

					} if(BINARY == 0 && CF_v_C == 0)
					{

						if(MULTIPLICATIVE_KERNEL == true)
						{
                                			thresholds[i][island] *= gsl_ran_lognormal(r, HARDCODED_ZETA, sqrt(-1*2*HARDCODED_ZETA));

						} else if(ADDITIVE_KERNEL == true)
						{
							double output = 0;
							int found = 0;
					
							while(found == 0)
							{

								output = gsl_ran_gaussian(r, tSIGMA);

								if(output > (-1)*thresholds[i][island])
								{

									found = 1;

								}

							}

							thresholds[i][island] += output;
						}

					}

                        	}

                	}

			if(MIGRATION_RATE > 0)
			{
				for(int island = 0; island < ISLANDS; island++)
				{

					if(gsl_ran_bernoulli(r, MIGRATION_RATE) == 1)
					{
						int migrating_individual = rand()%POPULATION;
						int replaced_individual = rand()%POPULATION;
						int migrating_island = rand()%ISLANDS;

						for(int j = 0; j < PHENOTYPE_DIMENSION; j++)
						{
							phenotypes[replaced_individual][j][island] = phenotypes[migrating_individual][j][migrating_island];
						}

						thresholds[replaced_individual][island] = thresholds[migrating_individual][migrating_island];
						scalers[replaced_individual][island] = scalers[migrating_individual][migrating_island];

					}	
	
        			}
			}
		}
	}
}
