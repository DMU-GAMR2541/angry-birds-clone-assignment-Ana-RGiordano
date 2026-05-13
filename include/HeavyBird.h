#pragma once
#include "Bird.h"

class HeavyBird : public Bird {
public:
	HeavyBird(b2Vec2 b2_posIn, b2World& b2_world, std::string spritePath, float f_scale)
		: Bird(b2_posIn, b2_world, spritePath, 1024, 1024, 15.0f, f_scale, 50.0f) {
	}

	void useAbility() override {
		launch(b2Vec2(0.0f, 4.0f));
		std::cout << "Heavy bird ability: causes more damage" << std::endl;
	}

};