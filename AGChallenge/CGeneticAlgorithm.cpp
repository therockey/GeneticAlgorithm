#include "CGeneticAlgorithm.h"

CGeneticAlgorithm::CGeneticAlgorithm(int population, double crossing, double mutation, CLFLnetEvaluator &eval)
	: popSize(population), crossProb(crossing), mutProb(mutation), evaluator(eval) {}

void CGeneticAlgorithm::initialize()
{
	CIndividual::setEvaluator(&evaluator);

	vector<int> genotype;
	for (int i = 0; i < popSize; i++) {
		genotype.clear();
		fillRandomly(genotype);
		population.push_back(new CIndividual());
		population[i]->setGenotype(genotype);
	}
}

void CGeneticAlgorithm::runIter()
{
	vector<CIndividual*> newPopulation;

}

void CGeneticAlgorithm::fillRandomly(vector<int>& gen)
{
	gen.resize((size_t)evaluator.iGetNumberOfBits());

	for (int ii = 0; ii < gen.size(); ii++)
	{
		gen.at(ii) = 1 + lRand(evaluator.iGetNumberOfValues(ii) - 1);
	}
}
