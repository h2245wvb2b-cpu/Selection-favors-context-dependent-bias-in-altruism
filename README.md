# Selection-favors-context-dependent-bias-in-altruism
Code to 1) run simulations for the evolution of altruism under different strategies (context dependent or not) 2) generate figures from the output on these simulations.

The simulations are run in C, in the code found in the folder c_codes. The codes as used to generate the figures appear in the subdirectories for each figure. A generic version of the code that is clean and commented without any extraneous functionalities appears in the subdirectory "cleaned_commented_code". Users are recommended to use the code in this subdirectory.

To run this simulation you will need access to GNU scientific library. Access to the library is set in Makefile and may need to be changed according to the user's set up.

To run a simulation:

1) Navigate to bash_script.sh, the phenotype dimension of the population is set here, and is generically set to 20 but can be changed.

2) Navigate to cooperation.h to set up other parameters. 

Some parameters are set explicitly in the command line including: number of generations simulated, iteration of the simulation (if copies are desired), selection strength, phenotype mutation distance (sigma squared for a Gaussian mutation distribution), strategy mutation (Bernoulli p), benefit (positive effect of being helped), cost (opportunity cost of helping others, see article), and survey (number of partners surveyed).

All other parameters are set in cooperation.h with explanatory comments.

3) Run: bash bash_script.sh

This will compile the code into an executable, e.g. "execute_sim20" (for 20 dimensions)

4) Run the simulation and specify preferred parameters.
Example:
./execute_sim20 30000 1 1 .001 10 1 14.223 0.5 1 > optimal_threshold_outputs.csv

This command will run a simulation with a population that has a phenotype dimension of 20, for 30000 generations. Selection strength is 1, the gaussian sigma squared mutation distance for the phenotype is set to 1, the Bernoulli p for a strategy mutation is .001, benefit is 10, cost is 1, the initial innate threshold is 14.223, the initial context threshold coefficient is 0.5, and the current iteration of the simulation is 1. EMPIRICAL_THRESH is cooperation.h is set to 1, so the simulation will output information about the optimal thresholds into the file "optimal_threshold_outputs.csv".
