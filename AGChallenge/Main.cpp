#include "Evaluator.h"
#include "Optimizer.h"
#include "Timer.h"
#include "CIndividual.h"

#include <exception>
#include <iostream>
#include <random>

using namespace TimeCounters;

using namespace std;

#define dMAX_TIME 0.5 * 60


void vRunExperiment(CLFLnetEvaluator &cConfiguredEvaluator)
{
	try
	{
		CTimeCounter c_time_counter;

		double d_time_passed;

		COptimizer c_optimizer(cConfiguredEvaluator);

		c_time_counter.vSetStartNow();

		c_optimizer.vInitialize();

		c_time_counter.bGetTimePassed(&d_time_passed);

		while (d_time_passed <= dMAX_TIME)
		{
			c_optimizer.vRunIteration();
			vector<int> best = *c_optimizer.pvGetCurrentBest();
			for (int i = 0; i < best.size(); i++) {
				std::cout << best[i] << " ";
			}
			cout << "\n" << endl;;

			c_time_counter.bGetTimePassed(&d_time_passed);
		}//while (d_time_passed <= MAX_TIME)
	}//try
	catch (exception &c_exception)
	{
		cout << c_exception.what() << endl;
	}//catch (exception &c_exception)
}//void vRunExperiment(const CEvaluator &cConfiguredEvaluator)



void  vRunLFLExperiment(CString  sNetName)
{
	CLFLnetEvaluator c_lfl_eval;
	c_lfl_eval.bConfigure(sNetName);
	vRunExperiment(c_lfl_eval);
	
}//void vRunRastriginExperiment(int iNumberOfBits, int iBitsPerFloat, int iMaskSeed)



void main(int iArgCount, char **ppcArgValues)
{
	
	CIndividual::setEvaluator(new CLFLnetEvaluator());
	
	random_device c_mask_seed_generator;
	int i_mask_seed = (int)c_mask_seed_generator();


	CString  s_test;
	vRunLFLExperiment("104b00");
	

	
}//void main(int iArgCount, char **ppcArgValues)