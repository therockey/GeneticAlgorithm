#include "CGeneticAlgorithm.h"
#include "Timer.h"
#include "Evaluator.h"
using namespace TimeCounters;

CGeneticAlgorithm::CGeneticAlgorithm(int population, double crossing, double mutation)
 {
	popSize = population;
	crossProb = crossing;
	mutProb = mutation;

	evaluator.bConfigure("104b00");
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
		if (iters % 10 == 0)
		{
			erasmus();
		}
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
	//erasmus to wiadomo du¿o seksiku hehe

	int ammountOfStudents = popSize/10;
	cout << "NIECH DZIEJE SIE ERASMUS!!\n";
	vector<CIndividual*> bestFromBoth = getSomeBestAndSortPopulations(ammountOfStudents);
	//they will match untill their heir is better and not spend money on useless degree
	vector<CIndividual*> allPerfectChildren; 
	vector<CIndividual*> perfectChildrenPair;
	vector<CIndividual*> tempPerfectChildrenPair;
	CIndividual* fstPopParent;
	CIndividual* sndPopParent;
	for (size_t j = 0; j < ammountOfStudents / 2; j++) {
		fstPopParent = bestFromBoth[j];
		for (size_t i = ammountOfStudents / 2;i < ammountOfStudents; i++) {
			sndPopParent = bestFromBoth[i];
			perfectChildrenPair.push_back(fstPopParent);
			perfectChildrenPair.push_back(sndPopParent);
			for (size_t i = 0; i < 10; i++)
			{
				tempPerfectChildrenPair = fstPopParent->cross(0.5, *sndPopParent);
				tempPerfectChildrenPair.insert(tempPerfectChildrenPair.end(), perfectChildrenPair.begin(), perfectChildrenPair.end());
				CIndividual::selectSortChildren(tempPerfectChildrenPair);
				
				perfectChildrenPair.clear();
				perfectChildrenPair.push_back(tempPerfectChildrenPair[0]);
				perfectChildrenPair.push_back(tempPerfectChildrenPair[1]);
			}
			allPerfectChildren.push_back(perfectChildrenPair[0]);
			allPerfectChildren.push_back(perfectChildrenPair[2]);
		}
		/*cout << "tu w erasmusie nastapilo" << j << "powt\n";*/
	}

	//now we have to cross them back with their folks they introduce diversity
	vector<CIndividual*> children;
	int parentFst;


	vector<int> visited;
	//first population
	for (int i = 0; i < popSize; i++)
		visited.push_back(i);
	random_shuffle(visited.begin(), visited.end());

	//for (int i=0;i<popSize;i++)
	//	cout<<visited[i]<<" ";
	int ammountOfPerfectChildren = ammountOfStudents * ammountOfStudents / 4 - 1;

	for (int i = 0; i < popSize; i++) {

		parentFst = visited.back();
		visited.pop_back();
		
		children = population1[parentFst]->cross(crossProb, *allPerfectChildren[popSize % ammountOfPerfectChildren]);
		delete children[1];

		delete population1[parentFst];

		population1[parentFst] = children[0];
	}
	//sec population
	for (int i = 0; i < popSize; i++)
		visited.push_back(i);
	random_shuffle(visited.begin(), visited.end());

	//for (int i=0;i<popSize;i++)
	//	cout<<visited[i]<<" ";


	for (int i = 0; i < popSize; i++) {

		parentFst = visited.back();
		visited.pop_back();

		children = population2[parentFst]->cross(crossProb, *allPerfectChildren[popSize % ammountOfPerfectChildren]);
		delete children[1];

		delete population2[parentFst];

		population2[parentFst] = children[0];
	}

}
vector<CIndividual*> CGeneticAlgorithm::getSomeBestAndSortPopulations(int ammount)
{
	
	vector<CIndividual*> someBest;
	//taken from first
	CIndividual::selectSortChildren(population1);
	CIndividual::selectSortChildren(population2);

	//taken form second
	someBest.insert(someBest.end(), population1.begin(), population1.begin() + ammount / 2);
	someBest.insert(someBest.end(), population2.begin(), population2.begin() + ammount / 2);
	return someBest;
}
