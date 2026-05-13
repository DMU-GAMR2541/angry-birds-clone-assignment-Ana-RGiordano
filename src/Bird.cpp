#include "Bird.h"
#include <iostream>

Bird::Bird(b2Vec2 b2_posIn, b2World& b2_world, std::string spritePath,
	int i_frameWidthIn, int i_frameHeightIn, 
	float f_radiusIn, float f_scaleIn, float f_attackValueIn)
	: GameObject(spritePath, sf::Vector2f(b2_posIn.x * f_scaleIn, b2_posIn.y * f_scaleIn)),
	f_scale(f_scaleIn),
	f_radius(f_radiusIn),
	f_attackValue(f_attackValueIn),
	str_spriteLocation(spritePath)
{
	if (!sf_tex.loadFromFile(str_spriteLocation)) {
		std::cout << "Failed to load bird texture: " << str_spriteLocation << std::endl;
	}

	sp_rendered.setTexture(sf_tex);
	sp_rendered.setTextureRect(sf::IntRect(0, 0, i_frameWidthIn, i_frameHeightIn));
	sp_rendered.setOrigin(i_frameWidthIn / 2.0f, i_frameHeightIn / 2.0f);

	float f_diameter = f_radius * 2.0f;

	sp_rendered.setScale(
		f_diameter / i_frameWidthIn,
		f_diameter / i_frameHeightIn
	);

	b2_bodyDef.type = b2_dynamicBody;
	b2_bodyDef.position = b2_posIn;

	b2_body = b2_world.CreateBody(&b2_bodyDef);

	b2_circleShape.m_radius = f_radius / f_scale;

	b2_fixtureDef.shape = &b2_circleShape;
	b2_fixtureDef.density = 1.0f;
	b2_fixtureDef.friction = 0.3f;
	b2_fixtureDef.restitution = 0.4f;

	b2_body->CreateFixture(&b2_fixtureDef);
		
}


void Bird::update() {
	b2Vec2 pos = b2_body->GetPosition();

	sp_rendered.setPosition(pos.x * f_scale, pos.y * f_scale);
	sp_rendered.setRotation(b2_body->GetAngle() * (180.0f / 3.14159265f));
}

void Bird::draw(sf::RenderWindow& window) {
	window.draw(sp_rendered);
}

float Bird::getAttackValue() {
	return f_attackValue;
}

void Bird::launch(b2Vec2 impulse) {
	b2_body->ApplyLinearImpulseToCenter(impulse, true);
}

b2Body* Bird::getBody() {
	return b2_body;
}
