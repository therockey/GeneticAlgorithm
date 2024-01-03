#include "CIndividual.h"
#include "Evaluator.h"

using namespace std;

CLFLnetEvaluator* CIndividual::evaluator = nullptr;

CIndividual::CIndividual()
{
	genotype = new vector<int>();
}

CIndividual::CIndividual(const int& genSize)
{
	genotype = new vector<int>(genSize);
}

CIndividual::CIndividual(const vector<int>& gen)
{
	genotype = new vector<int>(gen);
}

CIndividual::CIndividual(const CIndividual& other)
{
	genotype = new vector<int>(*other.genotype);
}

CIndividual::~CIndividual()
{
	delete genotype;
}



double CIndividual::dEvaluate() const
{
	return evaluator->dEvaluate(genotype);
}

CIndividual CIndividual::mutate(const double& MutProb)
{
	vector<int> mutatedGenotype;

	for (int i = 0; i < genotype->size(); i++) {
		if (dRand() < MutProb) {
			mutatedGenotype.push_back(lRand(evaluator->iGetNumberOfValues(i)));
		}
		else {
			mutatedGenotype.push_back((*genotype)[i]);
		}
	}

	return mutatedGenotype;
}

vector<CIndividual*> CIndividual::cross(const double& CrossProb, const CIndividual& other)
{
	vector<CIndividual*> result;
	
	if (dRand() < CrossProb) {
		int crossPoint = lRand(genotype->size() - 2);
		CIndividual* child1;
		CIndividual* child2;
		vector<int> childGenotype;


		// Crossing the first part of parent no.1 with second part of parent no.2
		for (int x = 0; x < genotype->size(); x++) {
			if (x < crossPoint) {
				childGenotype.push_back((*genotype)[x]);
			}
			else {
				childGenotype.push_back((*other.genotype)[x]);
			}
		}

		// Apply the resulting genotype to a child 
		child1 = new CIndividual(genotype->size());
		child1->setGenotype(childGenotype);

		childGenotype.clear();

		// Crossing the first part of parent no.2 with second part of parent no.1
		for (int x = 0; x < genotype->size(); x++) {
			if (x < crossPoint) {
				childGenotype.push_back((*other.genotype)[x]);
			}
			else {
				childGenotype.push_back((*genotype)[x]);
			}
		}

		// Apply the resulting genotype to a child
		child2 = new CIndividual(genotype->size());
		child2->setGenotype(childGenotype);

		result.push_back(new CIndividual(*this));
		result.push_back(new CIndividual(other));
		result.push_back(child1);
		result.push_back(child2);

		selectSortChildren(result);
		delete result.back();
		result.pop_back();
		delete result.back();
		result.pop_back();

	}
	else {
		result.push_back(new CIndividual(*this));
		result.push_back(new CIndividual(other));
	}

	return result;
}

void CIndividual::setEvaluator(CLFLnetEvaluator* eval)
{
	evaluator = eval;
}

void CIndividual::setGenotype(const vector<int>& other)
{
	delete genotype;
	genotype = new vector<int>(other);
}

int CIndividual::getGenotypeSize()
{
	return genotype->size();
}

vector<int> CIndividual::getGenotype()
{
	return *genotype;
}

void CIndividual::selectSortChildren(vector<CIndividual*> &other)
{
	int index;
	for (int i = 0; i < other.size()-1; i++) {
		index = i;
		for (int j = i + 1; j < other.size(); j++) {
			if (other[j]->dEvaluate() > other[index]->dEvaluate()) {
				index = j;
			}
		}

		if (index != i) swap(other[index], other[i]);
	
	}
}
