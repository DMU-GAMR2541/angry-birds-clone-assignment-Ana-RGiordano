#pragma once 
#include "Bird.h"

class RedBird : public Bird {
public:
	RedBird(b2Vec2 b2_posIn, b2World& b2_world, std::string spritePath, float f_scale)
		: Bird(b2_posIn, b2_world, spritePath, 1024, 1024, 15.0f, f_scale, 50.0f) {
		}

		void useAbility() override {
			std::cout << "Red bird ability: normal attack." << std::endl;
			}
};