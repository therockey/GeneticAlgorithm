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
#include "CGeneticAlgorithm.h"
#include "IslandModel.h"

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
	CLFLnetEvaluator evaluator;
	evaluator.bConfigure("g120d02");

	int populationSize = 40;
	double crossingProbability = 0.9;
	double mutationProbability = 0.4;

	CGeneticAlgorithm algo1(populationSize, crossingProbability, mutationProbability, evaluator);

	CGeneticAlgorithm algo2(40, 0.6, 0.3, evaluator);
	CGeneticAlgorithm algo3(40, 0.7, 0.2, evaluator);

	IslandModel islandModel(10, 5);
	islandModel.addIsland(&algo1);
	islandModel.addIsland(&algo2);
	islandModel.addIsland(&algo3);
	islandModel.runIteration(50);
	

	//runGAlgorithm( populationSize, crossingProbability, mutationProbability);


	
}