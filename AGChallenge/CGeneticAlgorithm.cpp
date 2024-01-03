#include "CGeneticAlgorithm.h"
#include "Timer.h"
using namespace TimeCounters;

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
	crossPop();
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
	for (int i = 1; i < population.size(); i++) {
		if (population[i]->dEvaluate() > best->dEvaluate()) {
			best = population[i];
		}
	}
	cout << best->dEvaluate() << endl;
	return best->getGenotype();
}

void CGeneticAlgorithm::fillRandomly(vector<int>& gen)
{
	gen.resize((size_t)evaluator.iGetNumberOfBits());

	for (int ii = 0; ii < gen.size(); ii++)
	{
		gen.at(ii) = 1 + lRand(evaluator.iGetNumberOfValues(ii) - 1);
	}
}

void CGeneticAlgorithm::crossPop()
{
	vector<CIndividual*> newGeneration;
	vector<CIndividual*> children;
	int parentFst;
	int parentSnd;

	for (int i = 0; i < popSize / 2; i++) {

		// cout << "crossing attempt no." << i+1 << endl;

		// randomize parent's no.1 index
		parentFst = lRand(popSize - 1);

		// keep randomizing parent's no.2 index until it's diffrent than parent no.1
		do {
			parentSnd = lRand(popSize - 1);
		} while (parentSnd == parentFst);

		// cout << "Parent 1: " << parentFst+1 << endl;
		// cout << "Parent 2: " << parentSnd+1 << endl;

		
		children = population[parentFst]->cross(crossProb, *population[parentSnd]);

		// cout << "Number of children: " << children.size() << endl;

		for (CIndividual* child : children) {
			newGeneration.push_back(child);
		}
		// cout << endl;
	}

	// cout << "old population size: " << population.size() << endl;
	// cout << "new generation size: " << newGeneration.size() << endl;

	for (CIndividual* old : population) {
		delete old;
	}
	population.clear();

	for (CIndividual* newIndividual : newGeneration) {
		population.push_back(newIndividual);
	}
	newGeneration.clear();
}
