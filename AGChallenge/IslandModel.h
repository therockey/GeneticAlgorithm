#pragma once
#include <vector>
#include "CGeneticAlgorithm.h"

class IslandModel{

public:
	IslandModel();
	IslandModel(int migFreq, int migSize);
	~IslandModel();
	void runTime(int maxTime);
	void runIteration(int iterations);
	void addIsland(CGeneticAlgorithm* island);
	
private:
	vector<CGeneticAlgorithm*> islands;
	int migrationFrequency;
	int migrationSize;

};