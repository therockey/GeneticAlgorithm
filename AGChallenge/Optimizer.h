#pragma once

#include "Evaluator.h"
#include "CIndividual.h"

#include <random>
#include <vector>

using namespace std;

class COptimizer
{
public:
	COptimizer(CLFLnetEvaluator &cEvaluator);

	void vInitialize();
	void vRunIteration();
	void simulate(int PopSize, double CrossProb, double MutProb);

	vector<int> *pvGetCurrentBest() { return &v_current_best; }

private:
	vector<CIndividual*> population;


	void v_fill_randomly(vector<int> &vSolution);

	CLFLnetEvaluator &c_evaluator;

	double d_current_best_fitness;
	vector<int> v_current_best;

	mt19937 c_rand_engine;
};//class COptimizer