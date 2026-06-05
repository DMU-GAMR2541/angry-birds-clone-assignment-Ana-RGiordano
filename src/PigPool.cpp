#include "PigPool.h"

void PigPool::addPig(Pig* pig){
	pigs.push_back(pig);
	pigUsed.push_back(false);
}

Pig* PigPool::getPigFromPool() {
	for (int i = 0; i < pigs.size(); i++) {
		if (!pigUsed[i]) { pigUsed[i] = true;
		return pigs[i];
		}
	}

	return nullptr;
}

void PigPool::returnPigToPool(Pig* pig) {
	for (int i = 0; i < pigs.size(); i++) {
		if (pigs[i] == pig) {
			pigUsed[i] = false;
			return;
		}
	}
}

int PigPool::getPoolSize() {
	return pigs.size();
}