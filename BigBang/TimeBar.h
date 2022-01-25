#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <tuple>

class TimeBar {
private:
	sf::RectangleShape scrollBar;
	sf::RectangleShape scrollSpace;
	int position = 0;

public:
	TimeBar(sf::VideoMode windowInfo, unsigned int size, unsigned int barSize) {
		auto width = windowInfo.width;
		auto height = windowInfo.height;

		scrollBar.setFillColor(sf::Color(0, 138, 0));
		scrollSpace.setFillColor(sf::Color(196, 196, 196));
		scrollBar.setSize(sf::Vector2f(barSize, size));
		scrollSpace.setSize(sf::Vector2f(width, size));
		scrollBar.setPosition(0, height-size);
		scrollSpace.setPosition(0, height-size);
	}
	std::vector<std::tuple<sf::Drawable*, unsigned int>> GetCompositions() {
		auto vec = std::vector<std::tuple<sf::Drawable*, unsigned int>>();
		vec.push_back(std::make_tuple(&scrollBar, 1U));
		vec.push_back(std::make_tuple(&scrollSpace, 0U));
		return vec;
	}
	int Current() { return position/120; }
	bool pressed = false;
	void Scroll(sf::RenderWindow* window, sf::Event event) {
		auto pos = sf::Mouse::getPosition(*window);
		if (event.type == sf::Event::MouseButtonPressed) {
			pressed = true;
		}
		if (event.type == sf::Event::MouseButtonReleased) {
			pressed = false;
		}
		if (pressed && pos.x > 0 && pos.x < window->getSize().x-scrollBar.getLocalBounds().width) {
			scrollBar.setPosition(pos.x, scrollBar.getPosition().y);
			position = pos.x;
		}
	}
};