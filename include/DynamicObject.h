#pragma once
#include "GameObject.h"
#include <box2d/box2d.h>

class DynamicObject : public GameObject{
protected:
	b2BodyDef   b2_bodyDef;
	b2FixtureDef   b2_fixtureDef;
	b2Body*   b2_body = nullptr;
	float   f_scale;
	float   f_radius;

public:
	DynamicObject() = default;
	DynamicObject(std::string str_spriteLoc, sf::Vector2f v2_position, float f_scaleIn, float f_radiusIn)
		: GameObject(str_spriteLoc, v2_position), f_scale(f_scaleIn), f_radius(f_radiusIn) {}
		
	virtual void update() = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual float getAttackValue() = 0;

	b2Body* getBody(){
		return b2_body;
	}

	virtual ~DynamicObject() = default;
};