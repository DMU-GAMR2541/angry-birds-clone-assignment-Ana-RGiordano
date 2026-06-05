#include "UI.h"
#include <iostream>

UI::UI() : StaticObject("", sf::Vector2f(0.0f, 0.0f), 30.0f) {
	if (!font.loadFromFile("../assets/fonts/angry-birds.ttf")) {
		std::cout << "Font failed to load" << std::endl;
	}

	titleText.setFont(font);
	titleText.setString("Annoyed Flocks");
	titleText.setCharacterSize(48);
	titleText.setFillColor(sf::Color::Red);
	titleText.setPosition(200.0f, 100.0f);

	loadingText.setFont(font);
	loadingText.setString("Loading...0%");
	loadingText.setCharacterSize(24);
	loadingText.setFillColor(sf::Color::Black);
	loadingText.setPosition(320.0f, 250.0f);

	loadingBarBackground.setSize(sf::Vector2f(400.0f, 30.0f));
	loadingBarBackground.setPosition(200.0f, 300.0f);
	loadingBarBackground.setFillColor(sf::Color::White);
	loadingBarFill.setSize(sf::Vector2f(0.0f, 30.0f));
	loadingBarFill.setPosition(200.0f, 300.0f);
	loadingBarFill.setFillColor(sf::Color::Green);

}

void UI::update() {

}

void UI::update(float progress) {
	if (progress < 0.0f) progress = 0.0f;
	if (progress > 100.0f) progress = 100.0f;

	loadingBarFill.setSize(sf::Vector2f(4.0f * progress, 30.0f));
	loadingText.setString("Loading..." + std::to_string(static_cast<int>(progress)) + "%");
}

void UI::draw(sf::RenderWindow& window) {
	window.draw(titleText);
	window.draw(loadingText);
	window.draw(loadingBarBackground);
	window.draw(loadingBarFill);
}

float UI::getAttackValue() {
	return 0.0f;
}
