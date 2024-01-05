#include "CGeneticAlgorithm.h"
#include "Timer.h"
#include "Evaluator.h"
using namespace TimeCounters;

CGeneticAlgorithm::CGeneticAlgorithm(int population, double crossing, double mutation)
	: evaluator(CLFLnetEvaluator())
 {
	popSize = population;
	crossProb = crossing;
	mutProb = mutation;

	evaluator.bConfigure("g120d02");
}

CGeneticAlgorithm::CGeneticAlgorithm(int population, double crossing, double mutation, CLFLnetEvaluator& eval)
	: evaluator(eval)
{
	popSize = population;
	crossProb = crossing;
	mutProb = mutation;
	evaluator = eval;

	//evaluator.bConfigure("g120d02");
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

	int iters = 0;
	//while (timeElapsed <= maxTime)
	while(iters < 2000)
	{
		runIter();
		counter.bGetTimePassed(&timeElapsed);
		iters++;
		if (iters % 5 == 0)
		{
			cout << iters << " ";
			getBestSolution();
			
		}/*if (iters % 20 == 0)
			mutProb *= 0.9;*/
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
	
	vector<CIndividual*> children;
	int parentFst;
	int parentSnd;


	vector<int> visited;
	for (int i = 0; i < popSize; i++)
		visited.push_back(i);
	random_shuffle(visited.begin(), visited.end());

	//for (int i=0;i<popSize;i++)
	//	cout<<visited[i]<<" ";

	for (int i = 0; i < popSize / 2; i++) {

		parentFst = visited.back();
		visited.pop_back();
		parentSnd = visited.back();
		visited.pop_back();
		/*parentFst = lRand(popSize - 1);

		do {
			parentSnd = lRand(popSize - 1);
		} while (parentSnd == parentFst);*/
	
		children = population[parentFst]->cross(crossProb, *population[parentSnd]);

		delete population[parentFst];
		delete population[parentSnd];

		population[parentFst]= children[0];
		population[parentSnd] = children[1];
	}
}

void CGeneticAlgorithm::mutatePop()
{
	double lastBest = population[population.size()-1]->dEvaluate();
	for (int i = 0; i < population.size(); i++) 
		if(population[i]->dEvaluate() > lastBest)
			lastBest = population[i]->dEvaluate();
		else {
			CIndividual mutated = population[i]->mutate(mutProb);
			if (mutated.dEvaluate() > population[i]->dEvaluate()) {
				delete population[i];
				population[i] = new CIndividual(mutated);
			}
		}
	
}
