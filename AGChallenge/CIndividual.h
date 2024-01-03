#pragma once
#include <vector>
#include "Evaluator.h"

using namespace std;

class CIndividual
{
public:
    CIndividual();
    CIndividual(const int& genSize);
    CIndividual(const CIndividual& other);
    ~CIndividual();
    double dEvaluate() const;
    CIndividual mutate(const double& MutProb);
    vector<CIndividual*> cross(const double& CrossProb, const CIndividual& other);
    static void setEvaluator(CLFLnetEvaluator* eval);
    void setGenotype(const vector<int>& other);
    int getGenotypeSize();
    vector<int> getGenotype();
private:
    vector<int>* genotype;
    static CLFLnetEvaluator* evaluator;

    void selectSortChildren(vector<CIndividual*>& other);
};
