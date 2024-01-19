#include "CIndividual.h"
#include "Evaluator.h"
#include "Evaluator.h"

using namespace std;

CIndividual::CIndividual(CLFLnetEvaluator& eval):evaluator(eval){
	genotype = new vector<int>();
}

CIndividual::CIndividual(const int& genSize, CLFLnetEvaluator& eval) :evaluator(eval){
	genotype = new vector<int>(genSize);
}

CIndividual::CIndividual(const vector<int>& gen, CLFLnetEvaluator& eval) :evaluator(eval){
	genotype = new vector<int>(gen);
}

CIndividual::CIndividual(const CIndividual& other, CLFLnetEvaluator& eval) :evaluator(eval){
	genotype = new vector<int>(*other.genotype);
}

CIndividual::~CIndividual(){
	delete genotype;
}



double CIndividual::dEvaluate() const{
	return evaluator.dEvaluate(genotype);
}

CIndividual CIndividual::mutate(const double& MutProb){
	vector<int> mutatedGenotype;

	for (int i = 0; i < genotype->size(); i++)
		if (dRand() < MutProb)
			mutatedGenotype.push_back(lRand(evaluator.iGetNumberOfValues(i)));
		else
			mutatedGenotype.push_back((*genotype)[i]);

	return {mutatedGenotype, evaluator};
}

pair<vector<int>*, vector<int>*>* CIndividual::onePointCrossover(const CIndividual& other){
	vector<int> childGenotype1;
	vector<int> childGenotype2;

	int crossPoint = lRand(genotype->size() - 2);

	for (int x = 0; x < genotype->size(); x++)
		if (x < crossPoint) {
			childGenotype1.push_back((*genotype)[x]);
			childGenotype2.push_back((*other.genotype)[x]);
		}
		else {
			childGenotype1.push_back((*other.genotype)[x]);
			childGenotype2.push_back((*genotype)[x]);
		}

	return new pair<vector<int>*, vector<int>*>(new vector<int>(childGenotype1), new vector<int>(childGenotype2));
}

pair<vector<int>*, vector<int>*>* CIndividual::shuffleCrossover(const CIndividual& other){
	vector<int> childGenotype1;
	vector<int> childGenotype2;

	for (int x = 0; x < genotype->size(); x++)
		if (lRand(10) >= 5) {
			childGenotype1.push_back((*genotype)[x]);
			childGenotype2.push_back((*other.genotype)[x]);
		}
		else {
			childGenotype1.push_back((*other.genotype)[x]);
			childGenotype2.push_back((*genotype)[x]);
		}

	return new pair<vector<int>*, vector<int>*>(new vector<int>(childGenotype1), new vector<int>(childGenotype2));
}

vector<CIndividual*> CIndividual::cross(const double& CrossProb, const CIndividual& other){
	vector<CIndividual*> result;
	result.push_back(new CIndividual(*this, evaluator));
	result.push_back(new CIndividual(other, evaluator));

	if (dRand() >= CrossProb)
		return result;

	CIndividual* child1;
	CIndividual* child2;
	vector<int> childGenotype1;
	vector<int> childGenotype2;
	child1 = new CIndividual(genotype->size(), evaluator);
	child2 = new CIndividual(genotype->size(), evaluator);
	
	//pair<vector<int>*, vector<int>*>* childern = onePointCrossover(other);
	pair<vector<int>*, vector<int>*>* childern = shuffleCrossover(other);
	
	child1->setGenotype(*childern->first);
	child2->setGenotype(*childern->second);

	result.push_back(child1);
	result.push_back(child2);

	selectSortChildren(result);
	
	delete result.back();
	result.pop_back();
	delete result.back();
	result.pop_back();

	return result;
}



void CIndividual::setGenotype(const vector<int>& other){
	delete genotype;
	genotype = new vector<int>(other);
}

int CIndividual::getGenotypeSize(){
	return genotype->size();
}

vector<int> CIndividual::getGenotype(){
	return *genotype;
}

void CIndividual::selectSortChildren(vector<CIndividual*>& other){
	for (int i = 0; i < other.size() - 1; i++) {
		int index = i;
		for (int j = i + 1; j < other.size(); j++) 
			if (other[j]->dEvaluate() > other[index]->dEvaluate()) 	
				index = j;

		if (index != i) 
			swap(other[index], other[i]);
	}
}




	/*
	OLD CODE	
	//ONE POINT CROSSOVER
	int crossPoint = lRand(genotype->size() - 2);

	for(int x = 0; x < genotype->size(); x++)
		if (x < crossPoint) {
			childGenotype1.push_back((*genotype)[x]);
			childGenotype2.push_back((*other.genotype)[x]);
		}
		else {
			childGenotype1.push_back((*other.genotype)[x]);
			childGenotype2.push_back((*genotype)[x]);
		}

	*/
	//----------------------------------------------------------------------------------------//


	//SHUFFLE CROSSOVER 0.5 - 0.5
	/*
	for (int x = 0; x < genotype->size(); x++)
		if (lRand(10) >= 5) {
			childGenotype1.push_back((*genotype)[x]);
			childGenotype2.push_back((*other.genotype)[x]);
		}
		else {
			childGenotype1.push_back((*other.genotype)[x]);
			childGenotype2.push_back((*genotype)[x]);
		}
	*/

	/*child1->setGenotype(childGenotype1);
		child2->setGenotype(childGenotype2);

		result.push_back(child1);
		result.push_back(child2);*/
	//----------------------------------------------------------------------------------------//
