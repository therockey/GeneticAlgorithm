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



void runGAlgorithm( int popSize, double crossProb, double mutProb, int islandAmmount) {

	CGeneticAlgorithm algo(popSize, crossProb, mutProb, islandAmmount);

	algo.initialize();

	algo.getBestSolutionFromAllIslands();

	algo.run(dMAX_TIME);

	algo.getBestSolutionFromAllIslands();
}

void main(int iArgCount, char **ppcArgValues)
{
	int populationSize = 100;
	double crossingProbability = 0.9;
	double mutationProbability = 0.4;
	int islandAmmount = 10;

	runGAlgorithm( populationSize, crossingProbability, mutationProbability, islandAmmount);
	
}