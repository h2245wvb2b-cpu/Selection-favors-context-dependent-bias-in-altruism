/*Set the initial phenotype and strategy distribution */
#include <stdlib.h>
#include "cooperation.h"

void initialize()
{

			for(int i = 0; i < POPULATION; i++)
        		{


				acting_thresholds[i] = 0;				
/*set initial phenotypes. */	
	        	        for(int j = 0; j < PHENOTYPE_DIMENSION; j++)
	        	        {

	        	                phenotypes[i][j] = INITIAL_PHENOTYPE;
	        	        }

/*set initial strategies. */
				double in1 = (double)rand() / RAND_MAX;
				thresholds[i] = in1*INITIAL_CONTEXTFREE_THRESH*2.0;

				double in2 = (double)rand() / RAND_MAX;
				scalers[i] = in2*INITIAL_CONTEXT_SCALER*2.0;

				thresholds[i] = INITIAL_CONTEXTFREE_THRESH;
				scalers[i] = INITIAL_CONTEXT_SCALER;
/* cf_or_c == 1 for context free, -1 for context. */
				cf_or_c[i] = 1;

/*add noise to thresholds/threshold coefficients. */

				if(SCATTER_CONTEXTFREE_THRESH != 0)
	                	{
	                	        if(i < POPULATION/2)
	                	        {
	                	                thresholds[i] *= (1 + (double)rand()/RAND_MAX*SCATTER_CONTEXTFREE_THRESH);
	                	        } else
	                	        {
	                	                thresholds[i] *= (1 - (double)rand()/RAND_MAX*SCATTER_CONTEXTFREE_THRESH);
	                	        }
	                	}

				if(SCATTER_CONTEXT_SCALER != 0)
				{
					if(i < POPULATION/2)
                        	        {
                        	                scalers[i] *= (1 + (double)rand()/RAND_MAX*SCATTER_CONTEXT_SCALER);
					} else
                                	{
                                	        scalers[i] *= (1 - (double)rand()/RAND_MAX*SCATTER_CONTEXT_SCALER);
                                	}
				}
			}
}
