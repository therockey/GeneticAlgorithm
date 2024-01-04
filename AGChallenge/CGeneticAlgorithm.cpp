#include "CGeneticAlgorithm.h"
#include "Timer.h"
#include "Evaluator.h"
#include "CIsland.h"
#include "CIndividual.h"
using namespace TimeCounters;

CGeneticAlgorithm::CGeneticAlgorithm(int population, double crossing, double mutation, int islandsCount)
 {
	popSize = population;
	crossProb = crossing;
	mutProb = mutation;
	islandAmmount = islandsCount;

	evaluator.bConfigure("g120d02");
}

void CGeneticAlgorithm::initialize()
{
	CIndividual::setEvaluator(&evaluator);

	vector<int> genotype;

	for (int i = 0; i < popSize; i++) {
		fillIslandsRandomly();
	}
}

void CGeneticAlgorithm::runIter()
{
	for (size_t i = 0; i < islandAmmount; i++)
	{
		crossPop(islands[i]->getPopPointer());
		mutatePop(islands[i]->getPopPointer());
	}
	if (lRand(100) == 1)
	{
		crossIslands();
	}
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
			getBestSolutionFromAllIslands();
			
		}/*if (iters % 20 == 0)
			mutProb *= 0.9;*/
	}
}

void CGeneticAlgorithm::fillIslandsRandomly()
{
	for (size_t i = 0; i < islandAmmount; i++)
	{
		islands.push_back(new CIsland(popSize));
		islands[i]->fillRandomly(evaluator);
	}
}

void CGeneticAlgorithm::crossIslands()
{
	CIsland* host = islands[lRand(islandAmmount - 1)];
	int œmia³kowie = 4;

	vector<CIndividual*>* takenFromIslands = new vector<CIndividual*>();

	//we'll take half best and half random
	for (size_t i = 0; i < islandAmmount; i++)
	{
		vector<CIndividual*> someBestCurrent = *islands[i]->getSomeBest(œmia³kowie/2);
		(*takenFromIslands).insert(takenFromIslands->end(), someBestCurrent.begin(), someBestCurrent.end());
		
		for (size_t j = 0; j < œmia³kowie/2; j++)
		{
			(*takenFromIslands).push_back(new CIndividual(*(*(islands[j]->getPopPointer()))[i]));
		}
	}
	vector<CIndividual*>* bestFromHost = host->getSomeBest(host->getPopSize() - œmia³kowie * islandAmmount);

	(*bestFromHost).insert(bestFromHost->end(), takenFromIslands->begin(), takenFromIslands->end());

	//Here we changed the whole population for the expeditions from other islands and the rest is best individuals from host
	host->changePopPointer(bestFromHost);
}


CIndividual* CGeneticAlgorithm::getBestSolutionForIsland(CIsland* island)
{
	vector<CIndividual*> population = *island->getPopPointer();
	CIndividual* best = population[0];
	for (int i = 1; i < population.size(); i++) 
		if (population[i]->dEvaluate() > best->dEvaluate()) 
			best = population[i];
	
	return best;
}

vector<int> CGeneticAlgorithm::getBestSolutionFromAllIslands()
{
	CIndividual* currentBest = getBestSolutionForIsland(islands[0]);
	for (size_t i = 1; i < islands.size(); i++)
	{
		if (getBestSolutionForIsland(islands[i])->dEvaluate() > currentBest->dEvaluate()) 
		{
			currentBest = getBestSolutionForIsland(islands[i]);
		}
	}
	cout << currentBest->dEvaluate() << endl;
	return currentBest->getGenotype();
}



void CGeneticAlgorithm::crossPop(vector<CIndividual*>* population)
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

		children = (*population)[parentFst]->cross(crossProb, *(*population)[parentSnd]);

		
		delete (*population)[parentFst];
		delete (*population)[parentSnd];

		(*population)[parentFst] = children[0];
		(*population)[parentSnd] = children[1];
	}
	
	

}

void CGeneticAlgorithm::mutatePop(vector<CIndividual*>* population)
{
	double lastBest = (*population)[population->size()-1]->dEvaluate();
	for (int i = 0; i < population->size(); i++) 
		if((*population)[i]->dEvaluate() > lastBest)
			lastBest = (*population)[i]->dEvaluate();
		else {
			CIndividual mutated = (*population)[i]->mutate(mutProb);
			if (mutated.dEvaluate() > (*population)[i]->dEvaluate()) {
				delete (*population)[i];
				(*population)[i] = new CIndividual(mutated);
			}
		}
	
}
