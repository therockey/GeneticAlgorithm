#include "CGeneticAlgorithm.h"
#include "Timer.h"
#include "Evaluator.h"
using namespace TimeCounters;

CGeneticAlgorithm::CGeneticAlgorithm(int population, double crossing, double mutation)
 {
	popSize = population;
	crossProb = crossing;
	mutProb = mutation;

	evaluator.bConfigure("g120d02");
}

void CGeneticAlgorithm::initialize()
{
	CIndividual::setEvaluator(&evaluator);

	vector<int> genotype;

	for (int i = 0; i < popSize; i++) {
		fillRandomly(genotype);
		population1.push_back(new CIndividual());
		population1[i]->setGenotype(genotype);
	}
	for (int i = 0; i < popSize; i++) {
		fillRandomly(genotype);
		population2.push_back(new CIndividual());
		population2[i]->setGenotype(genotype);
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
	CIndividual* best1 = population1[0];
	for (int i = 1; i < population1.size(); i++) 
		if (population1[i]->dEvaluate() > best1->dEvaluate()) 
			best1 = population1[i];

	CIndividual* best2 = population1[0];
	for (int i = 1; i < population1.size(); i++)
		if (population2[i]->dEvaluate() > best2->dEvaluate())
			best2 = population1[i];

	if (best2->dEvaluate() > best1->dEvaluate())
		best1 = best2;

	cout << best1->dEvaluate() << endl;
	return best1->getGenotype();
}

void CGeneticAlgorithm::fillRandomly(vector<int>& gen)
{
	gen.resize((size_t)evaluator.iGetNumberOfBits());

	for (int ii = 0; ii < gen.size(); ii++)
		gen.at(ii) = 1 + lRand(evaluator.iGetNumberOfValues(ii) - 1);
	
}


void CGeneticAlgorithm::crossPop()
{
	//first population
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

		children = population1[parentFst]->cross(crossProb, *population1[parentSnd]);

		
		delete population1[parentFst];
		delete population1[parentSnd];

		population1[parentFst]= children[0];
		population1[parentSnd] = children[1];
	}
	


	//second population



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

		children = population2[parentFst]->cross(crossProb/2, *population1[parentSnd]);


		delete population2[parentFst];
		delete population2[parentSnd];

		population2[parentFst] = children[0];
		population2[parentSnd] = children[1];
	}

}

void CGeneticAlgorithm::mutatePop()
{
	//first population
	double lastBest = population1[population1.size()-1]->dEvaluate();
	for (int i = 0; i < population1.size(); i++) 
		if(population1[i]->dEvaluate() > lastBest)
			lastBest = population1[i]->dEvaluate();
		else {
			CIndividual mutated = population1[i]->mutate(mutProb);
			if (mutated.dEvaluate() > population1[i]->dEvaluate()) {
				delete population1[i];
				population1[i] = new CIndividual(mutated);
			}
		}
	

	//second population
	lastBest = population2[population2.size() - 1]->dEvaluate();
	for (int i = 0; i < population2.size(); i++)
		if (population2[i]->dEvaluate() > lastBest)
			lastBest = population2[i]->dEvaluate();
		else {
			CIndividual mutated = population2[i]->mutate(mutProb*2);
			if (mutated.dEvaluate() > population2[i]->dEvaluate()) {
				delete population2[i];
				population2[i] = new CIndividual(mutated);
			}
		}
}

void CGeneticAlgorithm::erasmus()
{
	//each to each from some best individuals from both

}
