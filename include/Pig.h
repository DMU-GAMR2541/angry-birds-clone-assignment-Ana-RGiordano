#pragma once
#include "Enemy.h"
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include <string>

class Pig : public Enemy {
private:
	sf::Sprite sp_rendered;
	sf::Texture sf_tex;

	b2BodyDef b2_bodyDef;
	b2FixtureDef  b2_fixtureDef;
	b2Body* b2_body = nullptr;
	b2CircleShape b2_dynamicCircle;

	float f_scale;
	float f_radius;
	int i_frameWidth;
	int i_frameHeight;

	std::string  str_SpriteLocation;

public:
	Pig(b2Vec2 b2_posIn, b2World& b2_world, std::string spritePath, sf::RenderWindow& sf_window,
		int i_health = 100,
		float f_radius = 15.0f,
		float f_scale = 30.0f,
		int i_frameWidth = 55,
		int i_frameHeight = 57);




	void update();
	void draw(sf::RenderWindow& window) override;
	float getAttackValue() override;
	void applyImpulse(b2Vec2 impulse);

	void takeHit(int damage);
	b2Body* getBody();


	virtual ~Pig() = default;

};  