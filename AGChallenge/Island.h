#pragma once
#include "CIndividual.h"
#include "Evaluator.h"
#include <vector>

class Island
{
public:


	Island(int population, double crossing, double mutation, CString algoName);

	CLFLnetEvaluator evaluator;

	vector<CIndividual*> population;
	int popSize;
	double crossProb;
	double mutProb;


	void initialize();
	void runIter();
	void run(int iters);
	vector<int> getBestSolution();

	void fillRandomly(vector<int>& gen);
	void crossPop();
	void mutatePop();

	pair<int, int> tournamentSelection();
	pair<int, int> randomSelection();
	pair<int, int> comunistSelection(vector<int> visited);
};
