#include "Island.h"
#include "CGeneticAlgorithm.h"
#include "Timer.h"
#include "Evaluator.h"
#include <thread>
using namespace TimeCounters;



Island::Island(int population, double crossing, double mutation, CString algoName) {

	popSize = population;
	crossProb = crossing;
	mutProb = mutation;

	evaluator.bConfigure(algoName);
}


void Island::initialize() {

	vector<int> genotype;

	for (int i = 0; i < popSize; i++) {
		fillRandomly(genotype);
		population.push_back(new CIndividual(evaluator));
		population[i]->setGenotype(genotype);
	}
}

void Island::runIter() {
	crossPop();
	mutatePop();
}

void Island::run(int iters) {
	CTimeCounter counter;
	double timeElapsed;

	counter.vSetStartNow();
	counter.bGetTimePassed(&timeElapsed);

	int iterscurr = 0;

	while (iterscurr < iters) {
		runIter();
		counter.bGetTimePassed(&timeElapsed);
		iterscurr++;
	}
}


vector<int> Island::getBestSolution() {
	CIndividual* best = population[0];
	for (int i = 1; i < population.size(); i++)
		if (population[i]->dEvaluate() > best->dEvaluate())
			best = population[i];

	cout << best->dEvaluate() << endl;

	return best->getGenotype();
}

void Island::fillRandomly(vector<int>& genotype) {
	genotype.resize((size_t)evaluator.iGetNumberOfBits());

	for (int i = 0; i < genotype.size(); i++)
		genotype.at(i) = 1 + lRand(evaluator.iGetNumberOfValues(i) - 1);
}


pair<int, int> Island::tournamentSelection() {
	int parent1;
	int parent2;

	int candidate1;
	int candidate2;
	int candidate3;
	int candidate4;

	candidate1 = lRand(popSize - 1);
	do {
		candidate2 = lRand(popSize - 1);
	} while (candidate2 == candidate1);

	do {
		candidate3 = lRand(popSize - 1);
	} while (candidate3 == candidate1 || candidate3 == candidate2);

	do {
		candidate4 = lRand(popSize - 1);
	} while (candidate4 == candidate1 || candidate4 == candidate2 || candidate4 == candidate3);

	if (population[candidate1]->dEvaluate() < population[candidate2]->dEvaluate())
		parent1 = candidate1;
	else
		parent1 = candidate2;

	if (population[candidate3]->dEvaluate() < population[candidate4]->dEvaluate())
		parent2 = candidate3;
	else
		parent2 = candidate4;

	pair<int, int> parents;
	parents.first = parent1;
	parents.second = parent2;

	return parents;
}

pair<int, int> Island::randomSelection() {
	int parent1;
	int parent2;

	parent1 = lRand(popSize - 1);

	do {
		parent2 = lRand(popSize - 1);
	} while (parent2 == parent1);

	pair<int, int> parents;
	parents.first = parent1;
	parents.second = parent2;

	return parents;
}

pair<int, int> Island::comunistSelection(vector<int> visited) {
	int parent1;
	int parent2;

	parent1 = visited.back();
	visited.pop_back();
	parent2 = visited.back();
	visited.pop_back();

	pair<int, int> parents;
	parents.first = parent1;
	parents.second = parent2;

	return parents;
}


void Island::crossPop() {
	vector<CIndividual*> children;

	vector<int> visited;
	for (int i = 0; i < popSize; i++)
		visited.push_back(i);

	random_shuffle(visited.begin(), visited.end());

	vector<CIndividual*> nextPopulation;

	for (int i = 0; i < popSize / 2; i++) {
		//pair<int, int> parents = tournamentSelection();
		pair<int, int> parents = randomSelection();
		//pair<int, int> parents = comunistSelection(visited);

		children = population[parents.first]->cross(crossProb, *population[parents.second]);

		nextPopulation.push_back(children[0]);
		nextPopulation.push_back(children[1]);

		/*delete population[parents.first];
		delete population[parents.second];

		population[parents.first] = children[0];
		population[parents.second] = children[1];*/
	}
	population = nextPopulation;

}

void Island::mutatePop() {
	for (int i = 0; i < population.size(); i++) {
		CIndividual mutated = population[i]->mutate(mutProb);
		if (mutated.dEvaluate() > population[i]->dEvaluate()) {
			delete population[i];
			population[i] = new CIndividual(mutated, evaluator);
		}
	}

}