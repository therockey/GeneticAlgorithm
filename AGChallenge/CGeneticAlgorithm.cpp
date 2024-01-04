#include "CGeneticAlgorithm.h"
#include "Timer.h"
#include "Evaluator.h"
using namespace TimeCounters;

CGeneticAlgorithm::CGeneticAlgorithm(int population, double crossing, double mutation)
 {
	popSize = population;
	crossProb = crossing;
	mutProb = mutation;

	evaluator.bConfigure("104b00");
}

void CGeneticAlgorithm::initialize()
{
	CIndividual::setEvaluator(&evaluator);

	vector<int> genotype;

	for (int i = 0; i < popSize; i++) {
		fillRandomly(genotype);
		population.push_back(new CIndividual());
		population[i]->setGenotype(genotype);
	}
}

void CGeneticAlgorithm::runIter()
{
	crossPop();
	mutatePop();
}

void CGeneticAlgorithm::run(double maxTime)
{
	CTimeCounter counter;
	double timeElapsed;
	counter.vSetStartNow();
	counter.bGetTimePassed(&timeElapsed);


	while (timeElapsed <= maxTime)
	{
		runIter();
		counter.bGetTimePassed(&timeElapsed);
	}
}


vector<int> CGeneticAlgorithm::getBestSolution()
{
	CIndividual* best = population[0];
	for (int i = 1; i < population.size(); i++) 
		if (population[i]->dEvaluate() > best->dEvaluate()) 
			best = population[i];
	cout << best->dEvaluate() << endl;
	return best->getGenotype();
}

void CGeneticAlgorithm::fillRandomly(vector<int>& gen)
{
	gen.resize((size_t)evaluator.iGetNumberOfBits());

	for (int ii = 0; ii < gen.size(); ii++)
		gen.at(ii) = 1 + lRand(evaluator.iGetNumberOfValues(ii) - 1);
	
}

void CGeneticAlgorithm::crossPop()
{
	vector<CIndividual*> newGeneration;
	vector<CIndividual*> children;
	int parentFst;
	int parentSnd;

	for (int i = 0; i < popSize / 2; i++) {

		parentFst = lRand(popSize - 1);

		do {
			parentSnd = lRand(popSize - 1);
		} while (parentSnd == parentFst);


		
		children = population[parentFst]->cross(crossProb, *population[parentSnd]);

		
		delete population[parentFst];
		delete population[parentSnd];

		population[parentFst]= children[0];
		population[parentSnd] = children[1];
	}
}

void CGeneticAlgorithm::mutatePop()
{
	for (int i = 0; i < population.size(); i++) {
		CIndividual mutated = population[i]->mutate(mutProb);
		if (mutated.dEvaluate() > population[i]->dEvaluate()) {
			delete population[i];
			population[i] = new CIndividual(mutated);
		}
	}
}
