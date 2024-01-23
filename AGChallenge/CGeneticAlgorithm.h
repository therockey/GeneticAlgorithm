#pragma once
#include "CIndividual.h"
#include "Evaluator.h"
#include "Island.h""
#include <vector>

class CGeneticAlgorithm
{
public:
	CGeneticAlgorithm(CString algoName);
	void run(double time);
	vector<Island*> islands;
	void threadOperation(int i,int iters);
	int  populationSize;
};



