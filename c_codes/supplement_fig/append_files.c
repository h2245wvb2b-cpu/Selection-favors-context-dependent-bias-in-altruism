#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <gsl/gsl_statistics_double.h>
#include "cooperation.h"

void append_files(int time, int current_iteration, FILE **stat_output, FILE **fitness_output, FILE **parameter_output, FILE **lastgen_output)
{

	if(time == 0)
	{

/*Creating the files to output to. */

	        if(stat_on == true)
	        {

	                char stat_template[256];

			char parameter_template[256];

			if(BINARY == 1)
			{

				snprintf(stat_template, 256, "STATISTICS_HC_Dim%.2d_Pop%.2d_Gen%.2d_B%.2f_C%.2f_PM%.4f_SM%.4f_STR%.5f_GEN%.5f_iteration%d", PHENOTYPE_DIMENSION, POPULATION, endtime, BENEFIT, COST, PHENOTYPE_MUTATION, STRATEGY_MUTATION, STRICT_THRESH, GENEROUS_THRESH, current_iteration);

				snprintf(parameter_template, 256, "Parameters_STATISTICS_HC_Dim%.2d_Pop%.2d_Gen%.2d_B%.2f_C%.2f_PM%.4f_SM%.4f_STR%.5f_GEN%.5f_iteration%d", PHENOTYPE_DIMENSION, POPULATION, endtime, BENEFIT, COST, PHENOTYPE_MUTATION, STRATEGY_MUTATION, STRICT_THRESH, GENEROUS_THRESH, current_iteration);			
	
			} else if(BINARY == 0)
			{
				if(DEFECTORS != 1 && CF_v_C != 0)
				{

if(CONTINUATION != 1){		snprintf(stat_template, 256, "Context_STATISTICS_Dim%.3d_Pop%.2d_Gen%.2d_B%.2f_C%.2f_PM%.4f_SM%.4f_ICthresh%.5f_ICscaler%.5f_Tsigma%.5f_Ssigma%.5f_invadingscaler%.5f_iteration%d", PHENOTYPE_DIMENSION, POPULATION, endtime, BENEFIT, COST, PHENOTYPE_MUTATION, STRATEGY_MUTATION, INITIAL_CONTEXTFREE_THRESH, INITIAL_CONTEXT_SCALER, tSIGMA, sSIGMA, INVADING_CONTEXT_SCALER, current_iteration);}

if(CONTINUATION == 1){				snprintf(stat_template, 256, "Context_STATISTICS_Dim%.3d_Pop%.2d_Gen%.2d_B%.2f_C%.2f_PM%.4f_SM%.4f_ICthresh%.5f_ICscaler%.5f_Tsigma%.5f_Ssigma%.5f_iteration%d_testscalerIC%lf", PHENOTYPE_DIMENSION, POPULATION, endtime, BENEFIT, COST, PHENOTYPE_MUTATION, STRATEGY_MUTATION, INITIAL_CONTEXTFREE_THRESH, INITIAL_CONTEXT_SCALER, tSIGMA, sSIGMA, current_iteration, TEST_SCALER_IC);}

				snprintf(parameter_template, 256, "Parameters_Context_STATISTICS_HC_Dim%.3d_Pop%.2d_Gen%.2d_B%.2f_C%.2f_PM%.4f_SM%.4f_ICthresh%.5f_ICscaler%.5f_Tsigma%.5f_Ssigma%.5f_iteration%d", PHENOTYPE_DIMENSION, POPULATION, endtime, BENEFIT, COST, PHENOTYPE_MUTATION, STRATEGY_MUTATION, INITIAL_CONTEXTFREE_THRESH, INITIAL_CONTEXT_SCALER, tSIGMA, sSIGMA, current_iteration);

				} else if(DEFECTORS == 1)
				{
				snprintf(stat_template, 256, "STATISTICS_HC_Dim%.2d_Pop%.2d_Gen%.2d_B%.2f_C%.2f_PM%.4f_SM%.4f_InitialThresh%.5f_additivesigma%.5f_DM%.5f_InitialDefF%.5f_iteration%d", PHENOTYPE_DIMENSION, POPULATION, endtime, BENEFIT, COST, PHENOTYPE_MUTATION, STRATEGY_MUTATION, INITIAL_CONTEXTFREE_THRESH, tSIGMA, DEFECTOR_MUTATION, DEFECTOR_IC, current_iteration);

                        	snprintf(parameter_template, 256, "Parameters_STATISTICS_HC_Dim%.2d_Pop%.2d_Gen%.2d_B%.2f_C%.2f_PM%.4f_SM%.4f_InitialThresh%.5f_additivesigma%.5f_DM%.5f_InitialDefF%.5f_iteration%d", PHENOTYPE_DIMENSION, POPULATION, endtime, BENEFIT, COST, PHENOTYPE_MUTATION, STRATEGY_MUTATION, INITIAL_CONTEXTFREE_THRESH, tSIGMA, DEFECTOR_MUTATION, DEFECTOR_IC, current_iteration);

				}

			}

			*stat_output = fopen(stat_template, "w");

			fprintf(*stat_output, "time");

			for(int island = 0; island < ISLANDS; island++)
			{
				fprintf(*stat_output, ", num_GENEROUS, num_STRICTs, num_CF, num_C, avg_contextfree_thresh, std_contextfree_thresh, avg_context_scaler, std_scaler, DONATIONS, avg_acting_thresholds_context, out_of_phase_payoffs_CF, out_of_phase_C");

				for(int i = 0; i < PHENOTYPE_DIMENSION; i++)
                		{

                        		if(stat_on == true)
                        		{

                                		fprintf(*stat_output, ", PHENOTYPE_MEAN_%d, PHENOTYPE_VAR_%d", (i+1), (i+1));

                        		}
				}

				if(DEFECTORS == 1)
				{

					fprintf(*stat_output, ", num_DEFECTORS");
				}
			}

			*parameter_output = fopen(parameter_template, "w");

			fprintf(*parameter_output, "SELECTION STRENGTH %f\nSURVEY %d\nBURN IN TIME %d\nSTEP SIZE %d\nLOG TIME %d\nINITIAL CONTEXTFREE THRESH %f\nINITIAL PHENOTYPE %f\nSCATTER CONTEXTFREE THRESH PERCENTAGE %f\nSCATTER PHENOTYPE PERCENTAGE %f\nSIMULATION SEED %lu", SELECTION_STRENGTH, SURVEY, BURN_IN, STEP_SIZE, LOGTIME, INITIAL_CONTEXTFREE_THRESH, INITIAL_PHENOTYPE, SCATTER_CONTEXTFREE_THRESH, SCATTER_PHENOTYPE, simulationseed);

	        }

		if(fitnesses_on == true)
                {

                        char fitness_template[256];

                        if(BINARY == 1)
                        {

                        	snprintf(fitness_template, 256, "FITNESSES_HC_Dim%.2d_Pop%.2d_Gen%.2d_B%.2f_C%.2f_PM%.4f_SM%.4f_STR%.5f_GEN%.5f_iteration%d", PHENOTYPE_DIMENSION, POPULATION, endtime, BENEFIT, COST, PHENOTYPE_MUTATION, STRATEGY_MUTATION, STRICT_THRESH, GENEROUS_THRESH, current_iteration);


                        } if(BINARY == 0)
                        {

				snprintf(fitness_template, 256, "FITNESSES_Dim%.3d_Pop%.2d_Gen%.2d_B%.2f_C%.2f_PM%.4f_SM%.4f_ICthresh%.5f_ICscaler%.5f_Tsigma%.5f_Ssigma%.5f_iteration%d", PHENOTYPE_DIMENSION, POPULATION, endtime, BENEFIT, COST, PHENOTYPE_MUTATION, STRATEGY_MUTATION, INITIAL_CONTEXTFREE_THRESH, INITIAL_CONTEXT_SCALER, tSIGMA, sSIGMA, current_iteration);  

                        }

                        *fitness_output = fopen(fitness_template, "w");
		}


		for(int island = 0; island < ISLANDS; island++)
		{


			for(int i = 0; i < PHENOTYPE_DIMENSION; i++)
                	{

	        	        if(fitnesses_on == true)
	        	        {
	        	                fprintf(*fitness_output, "PHENOTYPE_%d, ", i+1);
	        	        }

	        	}
		
			if(fitnesses_on == true)
	        	{       
	                        
        			fprintf(*fitness_output, "TIME, INDIVIDUAL, DISTANCE_TO_MEAN, CONTEXTFREE_THRESH, CONTEXT_SCALER, SCORE, DONATIONS, acting_threshold, CF_or_C");
                	
        		}

		}

     	        if(stat_on == true)
		{
                        fprintf(*stat_output, "\n");
                }

		if(fitnesses_on == true)
		{
			fprintf(*fitness_output, "\n");
		}

	}else if(time <= endtime)
	{
		
		if(fitnesses_on == true && time%FITNESS_STEP_SIZE == 0)
               	{

			for(int island = 0; island < ISLANDS; island++)
			{

				double phenotype_mean[PHENOTYPE_DIMENSION];

				for (int i = 0; i < PHENOTYPE_DIMENSION; i++)
				{

					for (int j = 0; j < POPULATION; j++)
					{

						phenotype_mean[i] += phenotypes[j][i][island];

					}

					 phenotype_mean[i] = phenotype_mean[i]/((double) POPULATION);

				}

				for(int count = 0; count < 50; count++)
                        	{
					int i = rand()%POPULATION;

					double distance = 0;

                        	        for (int j = 0; j < PHENOTYPE_DIMENSION; j++)
                                	{

                                	        distance += (phenotypes[i][j][island] - phenotype_mean[j])*(phenotypes[i][j][island] - phenotype_mean[j]);
	
        	                                fprintf(*fitness_output, "%.3f, ", phenotypes[i][j][island]);
	
        	                        }

        	                        distance = sqrt(distance);

                	                fprintf(*fitness_output, "%d, %d, %.5f, %.5f, %.15f, %.5f, %d, %.15f, %d", time, i, distance, thresholds[i][island], scalers[i][island], results[i][island], donation_events[i][island], acting_thresholds[i][island], cf_or_c[i][island]);
					fprintf(*fitness_output, "\n");

				}

                        }


                }

		if(stat_on == true && time%STEP_SIZE == 0)
                {
			fprintf(*stat_output, "%d", time);			

			for(int island = 0; island < ISLANDS; island++)
			{

				int num_stricts = 0;
				int num_generous = 0;			

				if(BINARY == 1)
				{

                        	        for (int i = 0; i < POPULATION; i++)
        	                        {
	
        	                                if(thresholds[i][island] == STRICT_THRESH)
        	                                {
        	                                        num_stricts++;
        	                                } else
        	                                {
        	                                        num_generous++;
        	                                }

                	                };


				};

				if(COMPETE_TWO_SCALERS == 1)
                                {

                                        for (int i = 0; i < POPULATION; i++)
                                        {

						if(INVADING_CONTEXT_SCALER < INITIAL_CONTEXT_SCALER)
						{
							STRICT_THRESH = INVADING_CONTEXT_SCALER;
						} else
						{
							STRICT_THRESH = INITIAL_CONTEXT_SCALER;
						}

                                                if(scalers[i][island] == STRICT_THRESH)
                                                {
                                                        num_stricts++;
                                                } else
                                                {
                                                        num_generous++;
                                                }

                                        };


                                };

				int num_CF = 0;
				int num_C = 0;

				if(CF_v_C != 0)
				{

					for(int i = 0; i < POPULATION; i++)
					{

						if(cf_or_c[i][island] >= 0)
						{
							num_CF++;
						} else
						{
							num_C++;
						}
					};
				}

				double CF_mean = -500;
				double CF_std = -500;

				double CF_payoff_std = -500;

				if(num_CF > 0)
				{

					double justCF[num_CF];
					double CFpayoffs[num_CF];
					int it = 0;
					for (int i = 0; i < POPULATION; i++)
					{
						if(cf_or_c[i][island] >= 0)
						{
							justCF[it] = thresholds[i][island];
							CFpayoffs[it] = results[i][island];
							it++;
						}
					}
					CF_mean = gsl_stats_mean(justCF, 1, num_CF);
					CF_std = sqrt(gsl_stats_variance(justCF, 1, num_CF));
					CF_payoff_std = gsl_stats_mean(CFpayoffs, 1, num_CF);
				}

				double C_mean = -500;
				double C_std = -500;
				double acting_threshold_avg = 0;

				double C_payoff_std = -500;

				int dummyC = num_C;
				if(num_C == 0)
				{
					dummyC = 1;
				};			
			
				double justC[dummyC];

				if(num_C > 0)
				{
		
					double Cpayoffs[num_C];	
					int it = 0;
					for (int i = 0; i < POPULATION; i++)
					{
						if(cf_or_c[i][island] < 0)
						{
							justC[it] = scalers[i][island];
							Cpayoffs[it] = results[i][island];
							acting_threshold_avg += acting_thresholds[i][island];
							it++;
						}
					}

				acting_threshold_avg /= num_C;
				C_mean = gsl_stats_mean(justC, 1, num_C);
				C_std = sqrt(gsl_stats_variance(justC, 1, num_C));
				C_payoff_std = gsl_stats_mean(Cpayoffs, 1, num_C);				

				}			
				
				fprintf(*stat_output, ", %d, %d, %d, %d, %f, %f, %f, %f, %d, %f, %f, %f", num_generous, num_stricts, num_CF, num_C, CF_mean, CF_std, C_mean, C_std, total_donations[island], acting_threshold_avg, CF_payoff_std, C_payoff_std);

				for(int i = 0; i < PHENOTYPE_DIMENSION; i++)
				{

					double phenotype_dimension[POPULATION];

					for (int j = 0; j < POPULATION; j++)
					{
						phenotype_dimension[j] = phenotypes[j][i][island];
					}

					fprintf(*stat_output, ", %f, %f", gsl_stats_mean(phenotype_dimension, 1, POPULATION), gsl_stats_variance(phenotype_dimension, 1, POPULATION));
				}

				if(DEFECTORS == 1)
                                {
                                        int defector_num = 0;
                                        for(int i = 0; i < POPULATION; i++)
                                        {
                                                if(defector_list[i][island] == 1)
                                                {
                                                        defector_num++;
                                                }
                                        }

                                        fprintf(*stat_output, ", %d", defector_num);
                                }

			}

		fprintf(*stat_output, "\n");

		}
	
	}

	if(time == endtime)
	{

		if(TO_CONTINUE == 1)
		{

                        char lastgen_template[256];

			if(BINARY == 1)
                        {
                                
                                snprintf(lastgen_template, 256, "LASTGEN_HC_Dim%.2d_Pop%.2d_Gen%.2d_B%.2f_C%.2f_PM%.4f_SM%.4f_STR%.5f_GEN%.5f_iteration%d", PHENOTYPE_DIMENSION, POPULATION, endtime, BENEFIT, COST, PHENOTYPE_MUTATION, STRATEGY_MUTATION, STRICT_THRESH, GENEROUS_THRESH, current_iteration);
                                
                        } else if(BINARY == 0)
                        {       
                                if(DEFECTORS != 1)
                                {
					snprintf(lastgen_template, 256, "Context_LASTGEN_Dim%.3d_Pop%.2d_Gen%.2d_B%.2f_C%.2f_PM%.4f_SM%.4f_ICthresh%.5f_ICscaler%.5f_Tsigma%.5f_Ssigma%.5f_iteration%d", PHENOTYPE_DIMENSION, POPULATION, endtime, BENEFIT, COST, PHENOTYPE_MUTATION, STRATEGY_MUTATION, INITIAL_CONTEXTFREE_THRESH, INITIAL_CONTEXT_SCALER, tSIGMA, sSIGMA, current_iteration);  

                                } else if(DEFECTORS == 1)
                                {
					snprintf(lastgen_template, 256, "LASTGEN_HC_Dim%.2d_Pop%.2d_Gen%.2d_B%.2f_C%.2f_PM%.4f_SM%.4f_InitialThresh%.5f_additivesigma%.5f_DM%.5f_InitialDefF%.5f_iteration%d", PHENOTYPE_DIMENSION, POPULATION, endtime, BENEFIT, COST, PHENOTYPE_MUTATION, STRATEGY_MUTATION, INITIAL_CONTEXTFREE_THRESH, tSIGMA, DEFECTOR_MUTATION, DEFECTOR_IC, current_iteration);
                                
                                }

                        }


			*lastgen_output = fopen(lastgen_template, "w");

			fprintf(*lastgen_output, "%d\n", time);

			for(int island = 0; island < ISLANDS; island++)
			{

				for(int i = 0; i < POPULATION; i++)
				{
					for(int j = 0; j < PHENOTYPE_DIMENSION; j++)
                			{

                        			fprintf(*lastgen_output, "%f, ", phenotypes[i][j][island]);

                			}
				
					fprintf(*lastgen_output, "%lf", thresholds[i][island]);
					fprintf(*lastgen_output, ", %lf", scalers[i][island]);				
		
					fprintf(*lastgen_output, ", %d", cf_or_c[i][island]);

					if(DEFECTORS == 1)
					{
						fprintf(*lastgen_output, ", %d", defector_list[i][island]);
					}

					fprintf(*lastgen_output, "\n");
	
				}
			}	
                }




        	if(stat_on == true)
        	{
        	        fclose(*stat_output);
        	}

        	if(fitnesses_on == true)
        	{
        	        fclose(*fitness_output);
        	}
	}

}





