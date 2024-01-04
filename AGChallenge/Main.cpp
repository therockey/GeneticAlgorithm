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

#define dMAX_TIME 4 * 60



void runGAlgorithm(CString netName, int popSize, double crossProb, double mutProb) {

	CGeneticAlgorithm algo(popSize, crossProb, mutProb);

	algo.initialize();

	algo.getBestSolution();

	algo.run(dMAX_TIME);

	algo.getBestSolution();
}

void main(int iArgCount, char **ppcArgValues)
{
	CString networkName = "104b00";
	int populationSize = 50;
	double crossingProbability = 0.6;
	double mutationProbability = 0.1;

	runGAlgorithm(networkName, populationSize, crossingProbability, mutationProbability);
	
}