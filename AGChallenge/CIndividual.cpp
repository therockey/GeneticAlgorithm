#include "CIndividual.h"
#include "Evaluator.h"

using namespace std;

CIndividual::CIndividual()
{
	genotype = new vector<int>();
}

CIndividual::CIndividual(const int& genSize)
{
	genotype = new vector<int>(genSize);
}

double CIndividual::dEvaluate()
{
	return evaluator.dEvaluate(genotype);
}

CIndividual CIndividual::mutate(const double& MutProb)
{
	return CIndividual();
}

vector<CIndividual> CIndividual::cross(const double& CrossProb, const CIndividual& other)
{
	return vector<CIndividual>();
}

void CIndividual::setEvaluator(CLFLnetEvaluator& other)
{
	evaluator = other;
}
