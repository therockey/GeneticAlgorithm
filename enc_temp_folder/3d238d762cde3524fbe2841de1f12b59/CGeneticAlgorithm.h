#pragma once
#include "CIndividual.h"
#include "Evaluator.h"

class CGeneticAlgorithm
{
public:
	CGeneticAlgorithm(int population, double crossing, double mutation);
	void initialize();
	void runIter();
	void run(double maxTime);
	vector<int> getBestSolution();
private:
	CLFLnetEvaluator evaluator;
	vector<CIndividual*> population1;
	vector<CIndividual*> population2;
	int popSize; 
	double crossProb;
	double mutProb;
	
	void fillRandomly(vector<int>& gen);
	void crossPop();
	void mutatePop();
	void erasmus();

	//vector<CIndividual*> getSomeBestAndSortPopulations(int ammount);

};

