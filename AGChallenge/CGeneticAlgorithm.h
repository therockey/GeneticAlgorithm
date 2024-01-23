#pragma once
#include "CIndividual.h"
#include "Evaluator.h"
#include <vector>


class Island
{
	friend class IslandModel;
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

class CGeneticAlgorithm
{
public:
	CGeneticAlgorithm(CString algoName);
	void run(double time);
	vector<Island*> islands;
	void threadOperation(int i,int iters);
	int  populationSize;

	//CLFLnetEvaluator evaluator;
	//vector<CIndividual*> population;
	//int popSize; 
	//double crossProb;
	//double mutProb;
	
	/*void fillRandomly(vector<int>& gen);
	void crossPop();
	void mutatePop();*/
};



