#pragma once
#include "CIndividual.h"

class CGeneticAlgorithm
{
public:
	CGeneticAlgorithm(int population, double crossing, double mutation);
	void run();
	vector<int> getSolution();
private:
	CIndividual* population;
	int PopSize; 
	double CrossProb;
	double MutProb;
};

