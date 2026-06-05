#pragma once
#include "Pig.h"
#include <vector>

class PigPool {
private:
	std::vector<Pig*> pigs;
	std::vector<bool> pigUsed;

public:
	PigPool() = default;

	void addPig(Pig* pig);
	Pig* getPigFromPool();
	void returnPigToPool(Pig* pig);
	int getPoolSize();

};
