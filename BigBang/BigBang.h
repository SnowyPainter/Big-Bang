#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "Particle.h"

class BigBang :public sf::Drawable {
private:
	std::vector<Particle*> particles;
	std::vector<QuarkGroup*> quarks;
	sf::CircleShape space;
	const sf::Vector2f center;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(space);

		for (int i = 0; i < particles.size(); i++) {
			target.draw(*particles[i]);
		}
		for (int i = 0; i < quarks.size(); i++) {
			target.draw(*quarks[i]);
		}

	}
public:
	BigBang(sf::Vector2f centerPosition) :center(centerPosition) {
		space = sf::CircleShape(0);
		space.setPosition(centerPosition);
		space.setFillColor(sf::Color::Color(46, 46, 46));
	}
	void Expand(int radius) {
		radius = radius / 9;
		auto radius3sq = radius * radius * 3;
		space.setRadius(radius3sq);
		space.setPosition(center.x - radius3sq, center.y - radius3sq);
		for (int i = 0; i < particles.size(); i++) {
			particles[i]->Expand(center, radius);
		}
		for (int i = 0; i < quarks.size(); i++) {
			quarks[i]->Expand(center, radius);
		}
	}
	void AddParticle(Particle* p) {
		p->SetPosition(center.x, center.y);
		particles.push_back(p);
	}
	void AddQuark(QuarkGroup* q) {
		q->SetPosition(center.x, center.y);
		quarks.push_back(q);
	}
};