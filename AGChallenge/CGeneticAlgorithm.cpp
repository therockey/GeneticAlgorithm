#include "CGeneticAlgorithm.h"
#include "Timer.h"
#include "Evaluator.h"
#include <thread>
using namespace TimeCounters;

CGeneticAlgorithm::CGeneticAlgorithm(CString algoName)
{

	populationSize = 200;
	double crossingProbability = 0.9;
	double mutationProbability = 0.25;

	for (int i = 0;i < 10;i++)
	{
		double muProb =(double) 1 /(double) (i + 1);
		islands.push_back(new Island(populationSize, muProb, muProb, algoName));
	}
	
}

void CGeneticAlgorithm::run(double time)
{
	for (int i = 0;i < islands.size();i++) {
		islands[i]->initialize();
		islands[i]->getBestSolution();
	}
	vector<thread> threads;


	CTimeCounter counter;
	double timeElapsed;

	counter.vSetStartNow();
	counter.bGetTimePassed(&timeElapsed);
	int allit = 0;
	while (timeElapsed<=time)
	{

		int iters = 25;
		cout << endl << "This is the " << allit * iters << ". iteration!"<<endl << endl;

		for (int i = 0;i < islands.size();i++)
		{
			thread t(&CGeneticAlgorithm::threadOperation, this, i, iters);
			threads.push_back(std::move(t));
		}
		for (int i = 0; i < threads.size(); ++i) 
			threads[i].join();
		

		threads.clear();
		allit++;
		cout << endl;
		//cout << endl<<"after migration" << endl;

		for (int i = populationSize/7;i < populationSize/2;i++)
		{
			CIndividual* temp = islands[islands.size() - 1]->population[i];
			for (int j = 0;j < islands.size();j++)
			{
				CIndividual*  temp2 = temp;
				temp = islands[j]->population[i];
				islands[j]->population[i] = temp2;
			}

		}

		for (int i = 0;i < islands.size();i++) 
			islands[i]->getBestSolution();
		
		
		counter.bGetTimePassed(&timeElapsed);
		//cout << "Time elapsed: " << timeElapsed << endl;
	}
	
	
}

void CGeneticAlgorithm::threadOperation(int i,int iters)
{
		islands[i]->run(iters);
		islands[i]->getBestSolution();
}




Island::Island(int population, double crossing, double mutation,CString algoName)
 {
	popSize = population;
	crossProb = crossing;
	mutProb = mutation;

	evaluator.bConfigure(algoName);
}

void Island::initialize()
{
	//CIndividual::setEvaluator(&evaluator);

	vector<int> genotype;

	for (int i = 0; i < popSize; i++) {
		fillRandomly(genotype);
		population.push_back(new CIndividual(evaluator));
		population[i]->setGenotype(genotype);
	}
}

void Island::runIter()
{
	crossPop();
	mutatePop();
}

void Island::run(int iters)
{
	CTimeCounter counter;
	double timeElapsed;

	counter.vSetStartNow();
	counter.bGetTimePassed(&timeElapsed);

	int iterscurr = 0;
	//while (timeElapsed <= maxTime)
	while(iterscurr < iters)
	{
		runIter();
		counter.bGetTimePassed(&timeElapsed);
		iterscurr++;
		/*if (iterscurr % 5 == 0)
		{
			cout << iterscurr << " ";
			getBestSolution();
			
		}*/
		/*if (iters % 20 == 0)
			mutProb *= 0.9;*/
	}
}


vector<int> Island::getBestSolution()
{
	CIndividual* best = population[0];
	for (int i = 1; i < population.size(); i++) 
		if (population[i]->dEvaluate() > best->dEvaluate()) 
			best = population[i];
	cout << best->dEvaluate() << endl;
	return best->getGenotype();
}

void Island::fillRandomly(vector<int>& gen)
{
	gen.resize((size_t)evaluator.iGetNumberOfBits());

	for (int ii = 0; ii < gen.size(); ii++)
		gen.at(ii) = 1 + lRand(evaluator.iGetNumberOfValues(ii) - 1);
	
}


void Island::crossPop()
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

void Island::mutatePop()
{
	for (int i = 0; i < population.size(); i++){
			CIndividual mutated = population[i]->mutate(mutProb);
			if (mutated.dEvaluate() > population[i]->dEvaluate()) {
				delete population[i];
				population[i] = new CIndividual(mutated, evaluator);
			}
		}
	
}


