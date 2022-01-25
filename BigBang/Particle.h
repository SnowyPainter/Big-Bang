#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <functional>

enum class Nucleus {
	Proton,
	Neutron
};

class Particle : public sf::Drawable {
protected:
	std::vector<sf::CircleShape> circle;
	std::function<float(int)> move;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if(!Hide)
			for (int i = 0; i < circle.size(); i++)
				target.draw(circle[i]);
	}
	sf::CircleShape newCircle() {
		auto c = sf::CircleShape(this->circle[0].getRadius());
		c.setFillColor(sf::Color::White);
		return c;
	}
	float randomRange(int x, int x2) {
		static std::default_random_engine e;
		static std::uniform_real_distribution<> dis(x, x2); // rage 0 - 1
		return dis(e);
	}
	int direction = 1;
public:
	std::string Name;
	bool Hide = false;
	Particle(std::string name, int radius, float randomSeed) :Name(name) {
		auto circle = sf::CircleShape(radius);
		this->circle.push_back(circle);
		direction = (randomSeed > 0) ? -1 : 1;
		move = [a = randomSeed](int x)-> float {
			return a * x;
		};
	}
	void SetBackgroundColor(sf::Color color) {
		for (int i = 0; i < circle.size(); i++)
			circle[i].setFillColor(color);
	}
	void SetPosition(float x, float y) {
		circle[0].setPosition(x, y); 
	}
	void Expand(sf::Vector2f center, int x) {
		x = x * direction;
		SetPosition(center.x + x, center.y + move(x));
	}
};

class QuarkGroup : public Particle {
private:
	Nucleus nucleus;
	void transform2q() {
		if (this->circle.size() != 2) return;
		auto pos = circle[0].getPosition();
		circle[1].setPosition(pos.x, pos.y + circle[1].getRadius() * 2);
	}
	void transform3q() {
		if (this->circle.size() != 3) return;
		auto pos = circle[0].getPosition();
		auto r = circle[0].getRadius();
		if (nucleus == Nucleus::Proton) {
			circle[1].setPosition(pos.x + r * 2, pos.y);
			circle[2].setPosition(pos.x + r, pos.y + r);
		}
		else if (nucleus == Nucleus::Neutron) {
			circle[1].setPosition(pos.x - r, pos.y+r);
			circle[2].setPosition(pos.x + r, pos.y+r);
		}
		
	}
	void reform() {
		if (circle.size() == 2) transform2q();
		else if (circle.size() == 3) transform3q();
	}
public:
	QuarkGroup(Nucleus type, int radius, float randomSeed):Particle("Quark", radius, randomSeed) {
		nucleus = type;
	}
	
	void AddParticle() {
		if (this->circle.size() > 2) return;
		this->circle.push_back(newCircle());
		reform();
	}

	void PopParticle() {
		if (this->circle.size() == 1) return;
		this->circle.erase(this->circle.end() - 1);
		reform();
	}
	void SetPosition(float x, float y) {
		circle[0].setPosition(x, y);
		reform();
	}
	void Expand(sf::Vector2f center, int x) {
		x = x * direction;
		SetPosition(center.x + x, center.y + move(x));
		reform();
	}
};