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

#define dMAX_TIME 0.5* 60



void runGAlgorithm( int popSize, double crossProb, double mutProb) {

	CGeneticAlgorithm algo(popSize, crossProb, mutProb);

	algo.initialize();

	algo.getBestSolution();

	algo.run(dMAX_TIME);

	algo.getBestSolution();
}

void main(int iArgCount, char **ppcArgValues)
{
	int populationSize = 1000;
	double crossingProbability = 0.7;
	double mutationProbability = 0.25;

	runGAlgorithm( populationSize, crossingProbability, mutationProbability);
	
}