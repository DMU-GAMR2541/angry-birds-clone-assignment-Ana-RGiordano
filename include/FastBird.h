#pragma once 
#include "Bird.h"

class FastBird : public Bird{
public:
	FastBird(b2Vec2 b2_posIn, b2World& b2_world, std::string spritePath, float f_scale)
		: Bird(b2_posIn, b2_world, spritePath, 57, 57, 12.0f, f_scale, 35.0f){
	}

	void useAbility() override{
		launch(b2Vec2(4.0f, 0.0f));
		std::cout << "Fast bird ability: speed boost." << std::endl;
	}
};