#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "cooperation.h"

void interaction(int time)
{

/*store individuals to be interacted with */


		total_donations = 0;


		for (int i = 0; i < POPULATION; i++)
        	{
			int individual_donations = 0;

/* check distance to the partners (there are SURVEY number of them. */
        	        	for (int j = 0; j < SURVEY; j++)
        	        	{

					double distance_to_survey = 0;

					for (int k = 0; k < PHENOTYPE_DIMENSION; k++)
					{

						distance_to_survey += (phenotypes[i][k] - phenotypes[store[i][j]][k])*(phenotypes[i][k] - phenotypes[store[i][j]][k]);
	
					}

					distance_to_survey = sqrt(distance_to_survey);
/* if the distance to the partner is less than the threshold, increment the partners payoff by BENEFIT. */
					if(distance_to_survey < acting_thresholds[i])
					{
						results[store[i][j]] += BENEFIT;
			
						individual_donations += 1;
						total_donations += 1;

					} else
					{
/* if the distance is greater than the threshold, the individual increments their own payoff by +COST. */
						results[i] += COST;
			
					}

				}
	
			donation_events[i] = individual_donations;


		}

/*update the optima that the thresholds could have been in this process. */
	hypothetical_optimum(time);

}


