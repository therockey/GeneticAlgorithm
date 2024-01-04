#include "CIsland.h"
#include "Evaluator.h"
using namespace std;

CIsland::CIsland(int ppsz) 
{
	popSize = ppsz;
	population = new vector<CIndividual*>();
}
CIsland::~CIsland()
{
	for (size_t i = 0; i < popSize; i++)
	{
		delete (*population)[i];
	}
	delete population;
}
void CIsland::fillRandomly(CLFLnetEvaluator& evaluator)
{
	vector<int> gens;
	for (size_t i = 0; i < popSize; i++)
	{
		gens.clear();
		population->push_back(new CIndividual());
		
		gens.resize((size_t)evaluator.iGetNumberOfBits());

		for (int ii = 0; ii < gens.size(); ii++)
			gens.at(ii) = 1 + lRand(evaluator.iGetNumberOfValues(ii) - 1);
		(*population)[i]->setGenotype(gens);
	}
	

}
vector<CIndividual*>* CIsland::getPopPointer()
{
	return population;
}

vector<CIndividual*>* CIsland::getSomeBest(int ammount)
{
	vector<CIndividual*>* someBest = new vector<CIndividual*>(ammount);

	for (size_t i = 0; i < population->size(); i++) {
		bool exitFlag = true;
			for (size_t j = 0; j < someBest->size() && exitFlag; j++)
				if ((*someBest)[j]->dEvaluate() < (*population)[i]->dEvaluate())
				{
					(*someBest)[j] = (*population)[i];
					exitFlag = false;
				}
	}
	
	return someBest;
}

int CIsland::getPopSize()
{
	return popSize;
}

void CIsland::changePopPointer(vector<CIndividual*>* newPopulation)
{
	delete population;
	population = newPopulation;
}
