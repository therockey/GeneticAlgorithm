#pragma once
#include "CIndividual.h"
#include "CIsland.h"
#include "Evaluator.h"

class CGeneticAlgorithm
{
public:
	CGeneticAlgorithm(int population, double crossing, double mutation,int islandAmmount);
	void initialize();
	void runIter();
	void run(double maxTime);
	CIndividual* getBestSolutionForIsland(CIsland* island);
	vector<int> getBestSolutionFromAllIslands();

private:
	CLFLnetEvaluator evaluator;
	vector<CIsland*> islands;
	int islandAmmount;
	int popSize; 
	double crossProb;
	double mutProb;
	
	void fillIslandsRandomly();
	void crossIslands();
	void crossPop(vector<CIndividual*>* population);
	void mutatePop(vector<CIndividual*>* population);
};

