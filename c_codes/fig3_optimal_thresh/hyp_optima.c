#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "cooperation.h"

#include <gsl/gsl_sort.h>
#include <gsl/gsl_sort_vector.h>

void hypothetical_optimum(int time)
{

	if(time == 0)
	{
	printf("time, rand_individual, average_distance, cf_or_c, threshold, scalers, num_duplicates, T0, P0, part_1, T1, P1, part_2, T2, P2, part_3, T3, P3, part_4, T4, P4, part_5, T5, P5, part_6, T6, P6, part_7, T7, P7, part_8, T8, P8, part_9, T9, P9, part_10, T10, P10\n");
	}

		if(time%FITNESS_STEP_SIZE == 0 && EMPIRICAL_THRESH == 1 && (time > 10000 && time < 10500))
		{

		int island = 0;

			for(int i0 = 0; i0 < 1000; i0++)
			{
				int rand_individual = i0;
				int rand_type = cf_or_c[rand_individual][island];

				int num_A = 0;
				int results_copy[POPULATION];
				int As[POPULATION];
				int iterator = 0;

				for(int i = 0; i < POPULATION; i++)
				{

					As[i] = -100;

					if(cf_or_c[i][island] == rand_type && rand_type == 1)
					{
						if(thresholds[rand_individual][island] == thresholds[i][island])
						{
							num_A++;

							As[iterator] = i;
							iterator++;
						}
					}

					if(cf_or_c[i][island] == rand_type && rand_type == -1)
					{

						if(scalers[rand_individual][island] == scalers[i][island])
						{
							num_A++;

							As[iterator] = i;
							iterator++;
						}
					}

					results_copy[i] = results[i][island] + 1;

				}


				double ave_pairwise_dist = 0;

				double distances_to_partners[SURVEY];

				for(int i = 0; i < SURVEY; i++)
				{
					double distance_to_survey = 0;

					for(int j = 0; j < PHENOTYPE_DIMENSION; j++)
					{
						distance_to_survey += (phenotypes[rand_individual][j][island] - phenotypes[ store[rand_individual][i][island] ][j][island])*(phenotypes[rand_individual][j][island] - phenotypes[ store[rand_individual][i][island] ][j][island]);
					}

					distance_to_survey = sqrt(distance_to_survey);

					distances_to_partners[i] = distance_to_survey;

					ave_pairwise_dist += distance_to_survey;

					if(distance_to_survey < acting_thresholds[rand_individual][island])
					{
						results_copy[ store[rand_individual][i][island] ] -= BENEFIT;
					} else
					{
						results_copy[rand_individual] -= COST;
					}
				}

			results_copy[rand_individual] += 10*COST;

			ave_pairwise_dist /= SURVEY;

			size_t *ascending_distance = malloc(10);

			gsl_sort_index(ascending_distance, distances_to_partners, 1, SURVEY);

			double Wbar = 0;

			for (int i = 0; i < POPULATION; i++)
			{
				Wbar += results_copy[i];
			}	

			Wbar /= POPULATION;

			double delta_p = 0;
			double Wbar2 = 0;

			printf("%d, %d, %f, %d, %f, %f, %d", time, rand_individual, ave_pairwise_dist, cf_or_c[rand_individual][island], thresholds[rand_individual][island], scalers[rand_individual][island], num_A);

			int already_picked[SURVEY];
			for(int i = 0; i < SURVEY; i++)
			{
				already_picked[i] = -1;
			};

			for(int i = 0; i <= SURVEY; i++)
			{

				delta_p = 0;

				Wbar2 = Wbar + i*(BENEFIT-COST)/POPULATION;

				for(int k = 0; k < SURVEY; k++)
	                        {
	                        	if(already_picked[k] == 1)
	                                {
	                                	delta_p += BENEFIT/Wbar2;
	                                }
	                        }
				
				for(int j = 0; j < num_A; j++)
				{
					if(i != 0)
					{						
						if(As[j] == store[rand_individual][ascending_distance[i-1] ][island])
		                                {
							already_picked[i-1] = 1;
							delta_p += BENEFIT/Wbar2;
						}
					}
					delta_p += results_copy[j]/Wbar2 - results_copy[j]/Wbar;
				}
				
				delta_p -= i*COST/Wbar2;

				if(i < SURVEY)
				{
	
					printf(", %f, %f, %d", distances_to_partners[ ascending_distance[i] ], delta_p, store[rand_individual][i][island]);
				}

				if(i == SURVEY)
				{
					printf(", 1000, %f", delta_p);
				}
			}
		printf("\n");		

		free(ascending_distance);
		}

	}

}

