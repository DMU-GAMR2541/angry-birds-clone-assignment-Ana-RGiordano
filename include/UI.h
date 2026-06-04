#pragma once
#include <SFML/Graphics.hpp>

class UI {
private:
	sf::Font font;
	sf::Text titleText;
	sf::RectangleShape loadingBarBackground;
	sf::RectangleShape loadingBarFill;

public:
	UI();

	void update(float progress);
	void draw(sf::RenderWindow& window);


};