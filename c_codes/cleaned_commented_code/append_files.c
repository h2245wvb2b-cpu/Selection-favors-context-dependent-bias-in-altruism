#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <gsl/gsl_statistics_double.h>
#include "cooperation.h"

void append_files(int time, int current_iteration, FILE **stat_output, FILE **fitness_output)
{

	if(time == 0)
	{

/*Creating the files to output to. */

	        if(stat_on == true)
	        {

	                char stat_template[256];

			snprintf(stat_template, 256, "STATISTICS_Dim%.3d_Pop%.2d_Gen%.2d_B%.2f_C%.2f_PM%.4f_SM%.4f_ICthresh%.5f_ICscaler%.5f_iteration%d", PHENOTYPE_DIMENSION, POPULATION, endtime, BENEFIT, COST, PHENOTYPE_MUTATION, STRATEGY_MUTATION, INITIAL_CONTEXTFREE_THRESH, INITIAL_CONTEXT_SCALER, current_iteration);

			*stat_output = fopen(stat_template, "w");

			fprintf(*stat_output, "time");

			fprintf(*stat_output, ", num_CF, num_C, avg_contextfree_thresh, std_contextfree_thresh, avg_context_scaler, std_scaler, DONATIONS, avg_acting_thresholds_context, mean_payoffs_CF, mean_payoffs_C, all_thresholds, all_threshold_coefficients");

			for(int i = 0; i < PHENOTYPE_DIMENSION; i++)
                	{

                        	if(stat_on == true)
                        	{

                                	fprintf(*stat_output, ", PHENOTYPE_MEAN_%d, PHENOTYPE_VAR_%d", (i+1), (i+1));

                        	}
			}
	        }

		if(fitnesses_on == true)
                {

			char fitness_template[256];

			snprintf(fitness_template, 256, "FITNESSES_Dim%.3d_Pop%.2d_Gen%.2d_B%.2f_C%.2f_PM%.4f_SM%.4f_ICthresh%.5f_ICscaler%.5f_iteration%d", PHENOTYPE_DIMENSION, POPULATION, endtime, BENEFIT, COST, PHENOTYPE_MUTATION, STRATEGY_MUTATION, INITIAL_CONTEXTFREE_THRESH, INITIAL_CONTEXT_SCALER, current_iteration);  


                        *fitness_output = fopen(fitness_template, "w");

			for(int i = 0; i < PHENOTYPE_DIMENSION; i++)
                	{

				fprintf(*fitness_output, "PHENOTYPE_%d, ", i+1);

			}			
	                        
        		fprintf(*fitness_output, "TIME, INDIVIDUAL, DISTANCE_TO_MEAN, CONTEXTFREE_THRESH, CONTEXT_SCALER, SCORE, DONATIONS, acting_threshold, CF_or_C, average_distance_observed");
                	
		}

     	        if(stat_on == true)
		{
                        fprintf(*stat_output, "\n");
                }

		if(fitnesses_on == true)
		{
			fprintf(*fitness_output, "\n");
		}

	}

	if(time <= endtime)
	{


                for (int i = 0 ; i < POPULATION; i++)
                {

                        double average_distance = 0;

                        for (int j = 0; j < SURVEY; j++)
                        {

                                int rand_individual = rand()%POPULATION;

                                store[i][j] = rand_individual;

                                double distance_to_survey = 0;

                                for (int k = 0; k < PHENOTYPE_DIMENSION; k++)
                                {

                                        distance_to_survey += (phenotypes[i][k] - phenotypes[store[i][j]][k])*(phenotypes[i][k] - phenotypes[store[i][j]][k]);

                                }

                                distance_to_survey = sqrt(distance_to_survey);
                                average_distance += distance_to_survey;


                        }


                        average_distance /= SURVEY;

                        average_distances[i] = average_distance;

                        if(cf_or_c[i] <= 0)
                        {

                                acting_thresholds[i] = scalers[i]*average_distance;

                        } else if(cf_or_c[i] > 0)
                        {

                                acting_thresholds[i] = thresholds[i];

                        }


                        results[i] = 0;
                }

        }


	if(fitnesses_on == true && (time%FITNESS_STEP_SIZE == 0 && (time > 10000 && time < 10500)))
	{

		double phenotype_mean[PHENOTYPE_DIMENSION];

		for (int i = 0; i < PHENOTYPE_DIMENSION; i++)
		{

			for (int j = 0; j < POPULATION; j++)
			{

				phenotype_mean[i] += phenotypes[j][i];

			}

			phenotype_mean[i] = phenotype_mean[i]/((double) POPULATION);

		}

		for (int i = 0; i < POPULATION; i++)
		{

			double distance = 0;

			for (int j = 0; j < PHENOTYPE_DIMENSION; j++)
			{

				distance += (phenotypes[i][j] - phenotype_mean[j])*(phenotypes[i][j] - phenotype_mean[j]);
	
				fprintf(*fitness_output, "%.3f, ", phenotypes[i][j]);
	
			}

			distance = sqrt(distance);

			fprintf(*fitness_output, "%d, %d, %.5f, %.5f, %.15f, %.5f, %d, %.15f, %d, %.15f", time, i, distance, thresholds[i], scalers[i], results[i], donation_events[i], acting_thresholds[i], cf_or_c[i], average_distances[i]);

			fprintf(*fitness_output, "\n");

		}


	};

	if(stat_on == true && time%STEP_SIZE == 0)
	{

		fprintf(*stat_output, "%d", time);			

		int num_CF = 0;
		int num_C = 0;

		for(int i = 0; i < POPULATION; i++)
		{

			if(cf_or_c[i] >= 0)
			{
				num_CF++;
			} else
			{
				num_C++;
			}
		};

		double CF_mean = -500;
		double CF_std = -500;

		double CF_payoff_mean = -500;

		if(num_CF > 0)
		{

			double justCF[num_CF];
			double CFpayoffs[num_CF];
			int it = 0;
					
			for (int i = 0; i < POPULATION; i++)
			{
				if(cf_or_c[i] >= 0)
				{
					justCF[it] = thresholds[i];
					CFpayoffs[it] = results[i];
					it++;
				}
			}
					
			CF_mean = gsl_stats_mean(justCF, 1, num_CF);
			CF_std = sqrt(gsl_stats_variance(justCF, 1, num_CF));
			CF_payoff_mean = gsl_stats_mean(CFpayoffs, 1, num_CF);
		}

		double C_mean = -500;
		double C_std = -500;
		double acting_threshold_avg = 0;

		double C_payoff_mean = -500;

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
				if(cf_or_c[i] < 0)
				{
					justC[it] = scalers[i];
					Cpayoffs[it] = results[i];
					acting_threshold_avg += acting_thresholds[i];
					it++;
				}
			}

			acting_threshold_avg /= num_C;
			C_mean = gsl_stats_mean(justC, 1, num_C);
			C_std = sqrt(gsl_stats_variance(justC, 1, num_C));
			C_payoff_mean = gsl_stats_mean(Cpayoffs, 1, num_C);				

		}			
			
		double all_thresholds = 0;
		double all_threshold_coefficients = 0;
				
		for(int i = 0; i < POPULATION; i++)
		{
			all_thresholds += thresholds[i];
			all_threshold_coefficients += scalers[i];
		}

		all_thresholds /= (double)POPULATION;
		all_threshold_coefficients /= (double)POPULATION;

		fprintf(*stat_output, ", %d, %d, %f, %f, %f, %f, %d, %f, %f, %f, %f, %f", num_CF, num_C, CF_mean, CF_std, C_mean, C_std, total_donations, acting_threshold_avg, CF_payoff_mean, C_payoff_mean, all_thresholds, all_threshold_coefficients);

		for(int i = 0; i < PHENOTYPE_DIMENSION; i++)
		{

			double phenotype_dimension[POPULATION];

			for (int j = 0; j < POPULATION; j++)
			{
				phenotype_dimension[j] = phenotypes[j][i];
			}

			fprintf(*stat_output, ", %f, %f", gsl_stats_mean(phenotype_dimension, 1, POPULATION), gsl_stats_variance(phenotype_dimension, 1, POPULATION));
		}


	fprintf(*stat_output, "\n");

	}
	
	if(time == endtime)
	{

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

