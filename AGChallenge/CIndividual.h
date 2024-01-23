#pragma once
#include <vector>
#include "Evaluator.h"

using namespace std;

class CIndividual
{

public:
    CIndividual(CLFLnetEvaluator& eval);
    CIndividual(const int& genSize, CLFLnetEvaluator& eval);
    CIndividual(const vector<int>& gen, CLFLnetEvaluator& eval);
    CIndividual(const CIndividual& other, CLFLnetEvaluator& eval);
    ~CIndividual();

    pair<vector<int>*, vector<int>*>* onePointCrossover(const CIndividual& other);
    pair<vector<int>*, vector<int>*>* shuffleCrossover(const CIndividual& other);

    double dEvaluate() const;
    CIndividual mutate(const double& MutProb);
    vector<CIndividual*> cross(const double& CrossProb, const CIndividual& other);
    void setGenotype(const vector<int>& other);
    int getGenotypeSize();
    vector<int> getGenotype();
private:
    vector<int>* genotype;
    CLFLnetEvaluator& evaluator;
    void sortChildren(vector<CIndividual*>& other);
};
