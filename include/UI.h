#pragma once
#include "StaticObject.h"
#include <SFML/Graphics.hpp>

class UI : public StaticObject {
private:
	sf::Font font;
	sf::Text titleText;
	sf::Text loadingText;
	sf::RectangleShape loadingBarBackground;
	sf::RectangleShape loadingBarFill;

public:
	UI();

	void update()override;
	void update(float progress);
	void draw(sf::RenderWindow& window) override;
	float getAttackValue()override; // needed because GameObject requires getAttackValue
};