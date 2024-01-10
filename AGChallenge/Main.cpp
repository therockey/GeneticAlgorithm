#include "Evaluator.h"
#include "Optimizer.h"
#include "Timer.h"
#include "CIndividual.h"
#include "CGeneticAlgorithm.h"

#include <exception>
#include <iostream>
#include <random>

using namespace TimeCounters;

using namespace std;

#define dMAX_TIME 1.5* 60



void runGAlgorithm( CString algoName) {

	CGeneticAlgorithm algo(algoName);
	algo.run(dMAX_TIME);
}

void main(int iArgCount, char **ppcArgValues)
{


	runGAlgorithm( "104b00");
	
}