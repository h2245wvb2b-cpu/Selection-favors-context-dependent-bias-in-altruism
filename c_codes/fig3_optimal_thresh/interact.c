#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "cooperation.h"

void interaction(int time)
{

/*store individuals to be interacted with */


	for (int island = 0; island < ISLANDS; island++)
	{

		total_donations[island] = 0;

		int total_defectors = POPULATION;

		for (int i = 0; i < POPULATION; i++)
        	{
			int individual_donations = 0;

			if(DEFECTORS != 1 || (DEFECTORS == 1 && defector_list[i][island] != 1))
			{

				total_defectors -= 1;

        	        	for (int j = 0; j < SURVEY; j++)
        	        	{

					double distance_to_survey = 0;

					for (int k = 0; k < PHENOTYPE_DIMENSION; k++)
					{

						distance_to_survey += (phenotypes[i][k][island] - phenotypes[store[i][j][island]][k][island])*(phenotypes[i][k][island] - phenotypes[store[i][j][island]][k][island]);
	
					}

					distance_to_survey = sqrt(distance_to_survey);

					if(distance_to_survey < acting_thresholds[i][island])
					{

						results[store[i][j][island]][island] += BENEFIT;
			
						individual_donations += 1;
						total_donations[island] += 1;

					} else
					{

						results[i][island] += COST;
			
					}

				}
			} else
			{

				results[i][island] = COST*SURVEY;
			}
	
			donation_events[i][island] = individual_donations;


		}
	}


	hypothetical_optimum(time);

}


