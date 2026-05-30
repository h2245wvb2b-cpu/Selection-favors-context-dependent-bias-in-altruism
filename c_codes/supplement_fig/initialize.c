/*Set the initial phenotype and strategy distribution */
#include <stdlib.h>
#include "cooperation.h"

void initialize(FILE **previousdata)
{

	if(CONTINUATION == 1)
        {

		printf("made to initialize\n");

                char data_template[256];

		if(BINARY == 1)
                {
			snprintf(data_template, 256, "LASTGEN_Dim%.2d_Pop%.2d_Gen%.2d_B%.2f_C%.2f_PM%.4f_SM%.4f_STR%.5f_GEN%.5f_iteration%d", PHENOTYPE_DIMENSION, POPULATION, GENERATIONS, BENEFIT, COST, PHENOTYPE_MUTATION, STRATEGY_MUTATION, STRICT_THRESH, GENEROUS_THRESH, current_iteration);

			*previousdata = fopen(data_template, "r");

			fscanf(*previousdata, "%d\n", &new_start_time);			

			for(int island = 0; island < ISLANDS; island++)
			{

				for(int i = 0; i < POPULATION; i++)
                        	{
                                	for(int j = 0; j < PHENOTYPE_DIMENSION; j++)
                                	{
                                	        fscanf(*previousdata, "%lf, ", &phenotypes[i][j][island]);
                                	}

                                	fscanf(*previousdata, "%lf\n", &thresholds[i][island]);
                        	}

			}

		} else if(BINARY == 0)
                {
			if(DEFECTORS != 1 && CF_v_C == 0)
			{
				snprintf(data_template, 256, "LASTGEN_Dim%.2d_Pop%.2d_Gen%.2d_B%.2f_C%.2f_PM%.4f_SM%.4f_InitialThresh%.5f_additivesigma%.5f_iteration%d", PHENOTYPE_DIMENSION, POPULATION, endtime, BENEFIT, COST, PHENOTYPE_MUTATION, STRATEGY_MUTATION, INITIAL_CONTEXTFREE_THRESH, tSIGMA, current_iteration);
				
				*previousdata = fopen(data_template, "r");

				if(previousdata == NULL)
                                {
                                        printf("No previous data!\n");
                                        exit(0);
                                }
				fscanf(*previousdata, "%d\n", &new_start_time);

				for(int island = 0; island < ISLANDS; island++)
				{                        	
					for(int i = 0; i < POPULATION; i++)
                        		{
                        		        for(int j = 0; j < PHENOTYPE_DIMENSION; j++)
                        		        {
                        		                fscanf(*previousdata, "%lf, ", &phenotypes[i][j][island]);
							printf("%f, ", phenotypes[i][j][island]);	
                        		        }

                        		        fscanf(*previousdata, "%lf\n", &thresholds[i][island]);
                        		}
				}

			}if(DEFECTORS != 1 && CF_v_C != 0)
			{

				snprintf(data_template, 256, "Context_LASTGEN_Dim%.3d_Pop%.2d_Gen%.2d_B%.2f_C%.2f_PM%.4f_SM%.4f_ICthresh%.5f_ICscaler%.5f_Tsigma%.5f_Ssigma%.5f_iteration%d", PHENOTYPE_DIMENSION, POPULATION, GENERATIONS, BENEFIT, COST, PHENOTYPE_MUTATION, STRATEGY_MUTATION, INITIAL_CONTEXTFREE_THRESH, INITIAL_CONTEXT_SCALER, tSIGMA, sSIGMA, current_iteration);

				*previousdata = fopen(data_template, "r");

				if(previousdata == NULL)
				{
					printf("No previous data!\n");
					exit(0);
				}
				fscanf(*previousdata, "%d\n", &new_start_time);

				printf("new start time %d\n", new_start_time);

				for(int island = 0; island < ISLANDS; island++)
				{

					for(int i = 0; i < POPULATION; i++)
					{

						for(int j = 0; j < PHENOTYPE_DIMENSION; j++)
						{
							fscanf(*previousdata, "%lf, ", &phenotypes[i][j][island]);
						}

						fscanf(*previousdata, "%lf, ", &thresholds[i][island]);
						fscanf(*previousdata, "%lf, ", &scalers[i][island]);
						fscanf(*previousdata, "%d\n", &cf_or_c[i][island]);
						printf("%lf, %lf\n", thresholds[i][island], scalers[i][island]);

					}
				}


			} if(DEFECTORS == 1 && CF_v_C == 0)
			{
				snprintf(data_template, 256, "LASTGEN_Dim%.2d_Pop%.2d_Gen%.2d_B%.2f_C%.2f_PM%.4f_SM%.4f_InitialThresh%.5f_Tsigma%.5f_DM%.5f_InitialDefF%.5f_iteration%d", PHENOTYPE_DIMENSION, POPULATION, GENERATIONS, BENEFIT, COST, PHENOTYPE_MUTATION, STRATEGY_MUTATION, INITIAL_CONTEXTFREE_THRESH, tSIGMA, DEFECTOR_MUTATION, DEFECTOR_IC, current_iteration);

				*previousdata = fopen(data_template, "r");

				fscanf(*previousdata, "%d\n", &new_start_time);

				for(int island = 0; island < ISLANDS; island++)
				{

                        		for(int i = 0; i < POPULATION; i++)
                        		{
                        	        	for(int j = 0; j < PHENOTYPE_DIMENSION; j++)
                                		{
                                	        	fscanf(*previousdata, "%lf, ", &phenotypes[i][j][island]);
	                        		}

                                		fscanf(*previousdata, "%lf, ", &thresholds[i][island]);

                                		fscanf(*previousdata, "%d\n", &defector_list[i][island]);
                        		}


				}
			}
		}
        }else
	{
		
		for(int island = 0; island < ISLANDS; island++)
		{

			for(int i = 0; i < POPULATION; i++)
        		{

				acting_thresholds[i][island] = 0;				
	
	        	        for(int j = 0; j < PHENOTYPE_DIMENSION; j++)
	        	        {

	        	                phenotypes[i][j][island] = INITIAL_PHENOTYPE;
	        	        }

				if(BINARY == 1 && CONTEXT_FREE == 1)
				{              

					if(rand()%2 == 0)
					{     
						thresholds[i][island] = STRICT_THRESH;
					} else
					{       
						thresholds[i][island] = GENEROUS_THRESH;
					}
				} if(BINARY == 0)
				{

					defector_list[i][island] = -1;

					if(DEFECTORS == 1)
					{
						int defector_freq = DEFECTOR_IC*(double)POPULATION;
						//printf("def freq is %d\n", defector_freq);
						if(rand()%POPULATION <= defector_freq)
						{	
							defector_list[i][island] = 1;
						}
					}

					if(CF_v_C == 1)
	        	        	{
						thresholds[i][island] = INITIAL_CONTEXTFREE_THRESH;
						scalers[i][island] = INITIAL_CONTEXT_SCALER;

						cf_or_c[i][island] = 1;

	        	        	} else if(CF_v_C == 2)
        		        	{
        		        	        thresholds[i][island] = INITIAL_CONTEXTFREE_THRESH;
        		        	}else if(CF_v_C == 3)
        		        	{
        		        	        scalers[i][island] = INITIAL_CONTEXT_SCALER;
        		        	} else if(COMPETE_TWO_SCALERS == 1)
					{

						scalers[i][island] = INITIAL_CONTEXT_SCALER;

					}else
        		        	{
        		        	        printf("haven't added yet.");
        	        		        exit(0);
					}

				}

				if(SCATTER_CONTEXTFREE_THRESH != 0)
	                	{
	                	        if(i < POPULATION/2)
	                	        {
	                	                thresholds[i][island] *= (1 + (double)rand()/RAND_MAX*SCATTER_CONTEXTFREE_THRESH);
	                	        } else
	                	        {
	                	                thresholds[i][island] *= (1 - (double)rand()/RAND_MAX*SCATTER_CONTEXTFREE_THRESH);
	                	        }
	                	}

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
}
