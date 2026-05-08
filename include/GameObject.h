#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class GameObject {
private:
	sf::Vector2f v2_position;
	sf::Sprite sp_sprite;
	bool b_colliding = false;
	std::string str_spriteLoc;
	sf::Texture tx_texture;

public:
	GameObject() = default;
	GameObject(std::string& str_spriteLoc, sf::Vector2f& v2_position);

	//virtual functions
	virtual void update() = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual float getAttackValue() = 0;

	virtual ~GameObject() = default;

	sf::Vector2f getPosition() const {
	return v2_position;
	}
	bool isColliding() const {
	return b_colliding;
	}
	void setColliding(bool state) {
	b_colliding = state;
	}

	


};