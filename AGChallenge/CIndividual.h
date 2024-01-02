#pragma once
#include <vector>
#include "Evaluator.h"

using namespace std;

class CIndividual
{
public:
    CIndividual();
    CIndividual(const int& genSize);
    double dEvaluate();
    CIndividual mutate(const double& MutProb);
    vector<CIndividual> cross(const double& CrossProb, const CIndividual& other);
    static void setEvaluator(CLFLnetEvaluator* other);
private:
    vector<int>* genotype;
    static CLFLnetEvaluator* evaluator;
};
