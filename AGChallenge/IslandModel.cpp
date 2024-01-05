#include "IslandModel.h"
#include <thread>

using namespace std;

IslandModel::IslandModel(){
	migrationFrequency = 0;
	migrationSize = 0;
	
}

IslandModel::IslandModel(int migFreq, int migSize){
	migrationFrequency = migFreq;
	migrationSize = migSize;
	
}

IslandModel::~IslandModel(){
	
}

void IslandModel::runTime(int maxTime){

}

void IslandModel::runIteration(int iterations){
	vector<thread> threads;
	for (int i = 0; i < islands.size(); i++) {
		//islands[i]->initialize();
		threads.push_back(thread(&CGeneticAlgorithm::initialize, islands[i]));
	}
	for (int i = 0; i < islands.size(); i++) {
		threads[i].join();
	}
	threads.clear();

	int i = 0;
	while (i < iterations) {
		
		for (int j = 0; j < islands.size(); j++) {
			threads.push_back(thread(&CGeneticAlgorithm::runIter, islands[j]));
			threads[j].join();
		}
			
			//islands[j]->runIter();
		/*for (int j = 0; j < islands.size(); j++)
			threads[j].join();*/
		threads.clear();
		
		cout << i << endl;
		if (i % 5 == 0){
			
			for (int j = 0; j < islands.size(); j++) {
				islands[j]->getBestSolution();
			}
			cout << endl;
			
		}

		if (i % migrationFrequency == 0) {
			for (int j = 0; j < islands.size(); j++) {
				cout << "asd";
					CIndividual::selectSortChildren(islands[j]->population);
					cout << "asd";
			}
			for (int j = 0; j < islands.size(); j++) {
				
				for (int k = 0; k < migrationSize; k++) {
					//put the best migrationSize individuals from island j to island j+1 od the last migrationSize places
					islands[(j + 1) % islands.size()]->population[islands[j]->popSize - k - 1]->setGenotype(islands[j]->population[k]->getGenotype());
					////islands[j]->population[k]->setGenotype(islands[(j + 1) % islands.size()]->population[islands[j]->popSize - k - 1]->getGenotype());
				}
			}
		}
		i++;
	}
}

void IslandModel::addIsland(CGeneticAlgorithm* island){
	islands.push_back(island);
}
