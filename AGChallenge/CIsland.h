#pragma once
#include "CIndividual.h"

class CIsland
{
private:
	vector<CIndividual*>* population;
	int popSize;
	
public:
	CIsland(int popSize);
	~CIsland();
	void fillRandomly(CLFLnetEvaluator &evaluator);
	vector<CIndividual*>* getPopPointer();
	vector<CIndividual*>* getSomeBest(int ammount);
	int getPopSize();
	void changePopPointer(vector<CIndividual*>* newPopulation);
};

