#pragma once
#include "CIndividual.h"
#include "Evaluator.h"

class CGeneticAlgorithm
{
public:
	CGeneticAlgorithm(int population, double crossing, double mutation, CLFLnetEvaluator &eval);
	void initialize();
	void runIter();
	vector<int> getSolution();
private:
	CLFLnetEvaluator& evaluator;
	vector<CIndividual*> population;
	int popSize; 
	double crossProb;
	double mutProb;

	void fillRandomly(vector<int>& gen);
};

