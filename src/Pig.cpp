#include "Pig.h"
#include "Enemy.h"
#include <iostream>

Pig::Pig(b2Vec2 b2_posIn, b2World& b2_world, std::string spritePath, sf::RenderWindow& sf_window, 
	int i_health, 
	float f_radiusIn, 
	float f_scaleIn,
	int i_frameWidthIn,
	int i_frameHeightIn)
	: Enemy(i_health, spritePath, sf::Vector2f(b2_posIn.x * f_scaleIn, b2_posIn.y * f_scaleIn)),
	f_scale(f_scaleIn),
	f_radius(f_radiusIn),
	str_SpriteLocation(spritePath),
	i_frameWidth(i_frameWidthIn),
	i_frameHeight(i_frameHeightIn)


	//sfml 
{
	//texture
	if (!sf_tex.loadFromFile(str_SpriteLocation)) {
		std::cout << "Failed to load texture: " << str_SpriteLocation << std::endl;
	}

	sp_rendered.setTexture(sf_tex);
	sp_rendered.setTextureRect(sf::IntRect(0, 0, i_frameWidth, i_frameHeight)); // crops first frame from sprite sheet
	sp_rendered.setOrigin(i_frameWidth / 2.0f, i_frameHeight / 2.0f);
	

	//scaling sprite to match box2d
	float f_diameter = f_radius * 2.0f;
	sp_rendered.setScale(
		f_diameter / i_frameWidth,
		f_diameter / i_frameHeight
	);

	//box2d
	b2_bodyDef.type = b2_dynamicBody;
	b2_bodyDef.position = b2_posIn;

	b2_body = b2_world.CreateBody(&b2_bodyDef);

	b2_dynamicCircle.m_radius = f_radius / f_scale;

	b2_fixtureDef.shape = &b2_dynamicCircle;
	b2_fixtureDef.density = 1.0f;
	b2_fixtureDef.friction = 0.3f;
	b2_fixtureDef.restitution = 0.5f;

	b2_body->CreateFixture(&b2_fixtureDef);
}

void Pig::update() {
	if (checkIfPopped()) {
		if (b2_body) {
			b2_body->SetEnabled(false);
		}
		return;
	}
	b2Vec2 pos = b2_body->GetPosition();
	sp_rendered.setPosition(pos.x * f_scale, pos.y * f_scale);
	sp_rendered.setRotation(-b2_body->GetAngle() * (180.0f / 3.14159265f));
}

	void Pig::draw(sf::RenderWindow& sf_window) {
		if (checkIfPopped()) return;

		sf_window.draw(sp_rendered);
}

	//adding Damage

	void Pig::takeHit(int damage)
	{
		takeDamage(damage);
	}

	b2Body* Pig::getBody() {
		return b2_body;
	}

	void Pig::applyImpulse(b2Vec2 impulse) {
		b2_body->ApplyLinearImpulseToCenter(impulse, true);
}

	float Pig::getAttackValue() {
		return 0.0f;
	}