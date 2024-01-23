#include "CGeneticAlgorithm.h"
#include "Timer.h"
#include "Evaluator.h"
#include <thread>
using namespace TimeCounters;

CGeneticAlgorithm::CGeneticAlgorithm(CString algoName)
{

	populationSize = 50;
	double crossingProbability = 0.9;
	double mutationProbability = 0.25;
	islands.push_back(new Island(populationSize, 0.9, 0.3, algoName));

	for (int i = 0; i < 4;i++)
	{
		double muProb =(double) 1 /(double) (i + 1);
		islands.push_back(new Island(populationSize, muProb, dRand(), algoName));
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
	while (timeElapsed<=time){
		int iters = 25;
		cout << endl << "This is the " << allit * iters << ". iteration!"<<endl << endl;

		for (int i = 0;i < islands.size();i++){
			thread t(&CGeneticAlgorithm::threadOperation, this, i, iters);
			threads.push_back(std::move(t));
		}

		for (int i = 0; i < threads.size(); ++i) 
			threads[i].join();

		threads.clear();

		allit++;
		cout << endl;

		for (int i = populationSize/7;i < populationSize/2;i++){
			CIndividual* temp = islands[islands.size() - 1]->population[i];
			for (int j = 0;j < islands.size();j++){
				CIndividual*  temp2 = temp;
				temp = islands[j]->population[i];
				islands[j]->population[i] = temp2;
			}
		}

		for (int i = 0;i < islands.size();i++) 
			islands[i]->getBestSolution();
		
		counter.bGetTimePassed(&timeElapsed);
	}
	
	
}

void CGeneticAlgorithm::threadOperation(int i,int iters){
		islands[i]->run(iters);
		islands[i]->getBestSolution();
}




Island::Island(int population, double crossing, double mutation,CString algoName){
	popSize = population;
	crossProb = crossing;
	mutProb = mutation;

	evaluator.bConfigure(algoName);
}

void Island::initialize(){
	vector<int> genotype;

	for (int i = 0; i < popSize; i++) {
		fillRandomly(genotype);
		population.push_back(new CIndividual(evaluator));
		population[i]->setGenotype(genotype);
	}
}

void Island::runIter(){
	crossPop();
	mutatePop();
}

void Island::run(int iters){
	CTimeCounter counter;
	double timeElapsed;

	counter.vSetStartNow();
	counter.bGetTimePassed(&timeElapsed);

	int iterscurr = 0;
	
	while(iterscurr < iters){
		runIter();
		counter.bGetTimePassed(&timeElapsed);
		iterscurr++;
	}
}


vector<int> Island::getBestSolution(){
	CIndividual* best = population[0];
	for (int i = 1; i < population.size(); i++) 
		if (population[i]->dEvaluate() > best->dEvaluate()) 
			best = population[i];

	cout << best->dEvaluate() << endl;

	return best->getGenotype();
}

void Island::fillRandomly(vector<int>& gen){
	gen.resize((size_t)evaluator.iGetNumberOfBits());

	for (int ii = 0; ii < gen.size(); ii++)
		gen.at(ii) = 1 + lRand(evaluator.iGetNumberOfValues(ii) - 1);
}


pair<int, int> Island::tournamentSelection() {
	int parentFst;
	int parentSnd;

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

	//get 2 best parents
	if (population[candidate1]->dEvaluate() < population[candidate2]->dEvaluate())
		parentFst = candidate1;
	else
		parentFst = candidate2;

	if (population[candidate3]->dEvaluate() < population[candidate4]->dEvaluate())
		parentSnd = candidate3;
	else
		parentSnd = candidate4;

	pair<int, int> parents;
	parents.first = parentFst;
	parents.second = parentSnd;

	return parents;
}

pair<int, int> Island::randomSelection() {
	int parentFst;
	int parentSnd;

	parentFst = lRand(popSize - 1);

	do {
		parentSnd = lRand(popSize - 1);
	} while (parentSnd == parentFst);
	
	pair<int, int> parents;
	parents.first = parentFst;
	parents.second = parentSnd;

	return parents;
}

pair<int, int> Island::comunistSelection(vector<int> visited) {
	int parentFst;
	int parentSnd;

	parentFst = visited.back();
	visited.pop_back();
	parentSnd = visited.back();
	visited.pop_back();

	pair<int, int> parents;
	parents.first = parentFst;
	parents.second = parentSnd;

	return parents;
}


void Island::crossPop(){
	vector<CIndividual*> children;
	vector<int> visited;
	for (int i = 0; i < popSize; i++)
		visited.push_back(i);

	random_shuffle(visited.begin(), visited.end());

	for (int i = 0; i < popSize / 2; i++) {
		//pair<int, int> parents = tournamentSelection();
		pair<int, int> parents = randomSelection();
		//pair<int, int> parents = comunistSelection(visited);
		
		children = population[parents.first]->cross(crossProb, *population[parents.second]);


		delete population[parents.first];
		delete population[parents.second];

		population[parents.first]= children[0];
		population[parents.second] = children[1];
	}
}

void Island::mutatePop(){
	for (int i = 0; i < population.size(); i++){
			CIndividual mutated = population[i]->mutate(mutProb);
			if (mutated.dEvaluate() > population[i]->dEvaluate()) {
				delete population[i];
				population[i] = new CIndividual(mutated, evaluator);
			}
		}
	
}























/*
STARE METODY SELEKCJI
		//TOURNAMENT SELECTION
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

		//get 2 best parents
		if (population[candidate1]->dEvaluate() < population[candidate2]->dEvaluate())
			parentFst = candidate1;
		else
			parentFst = candidate2;

		if (population[candidate3]->dEvaluate() < population[candidate4]->dEvaluate())
			parentSnd = candidate3;
		else
			parentSnd = candidate4;
			*/
			//----------------------------------------------------------------------------------------//

			/*
			//EVERYONE HAS 2 CHILDREN
			parentFst = visited.back();
			visited.pop_back();
			parentSnd = visited.back();
			visited.pop_back();*/

			//----------------------------------------------------------------------------------------//

			/*
			//RANDOM SELECTION
			parentFst = lRand(popSize - 1);

			do {
				parentSnd = lRand(popSize - 1);
			} while (parentSnd == parentFst);
			*/
			//----------------------------------------------------------------------------------------//
			//children = population[parentFst]->cross(crossProb, *population[parentSnd]);





