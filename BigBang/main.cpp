#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <tuple>

#include "TimeBar.h"
#include "Error.h"

using namespace std;

sf::Font mainFont;

sf::Text newText(string text) {
    sf::Text t = sf::Text(text, mainFont, 20U);
    t.setFillColor(sf::Color::Black);
    return t;
}

int main() {
    ErrorReport e = ErrorReport("bigbang program error.txt");
    //pf
    if (!mainFont.loadFromFile("NotoSansKR-Medium.otf")) {
        e.WriteLine("Failed to load font NotoSansKR-Medium.otf");
        return 0;
    }

    auto windowVideoMode = sf::VideoMode(1000, 800);
    sf::RenderWindow window(windowVideoMode, "Big Bang", sf::Style::Titlebar | sf::Style::Close);
    
    TimeBar timeBar = TimeBar(windowVideoMode, 40, 100);
    auto drawables = vector<tuple<sf::Drawable*, unsigned int>>();
    auto compositions = timeBar.GetCompositions();
    drawables.insert(drawables.end(), compositions.begin(), compositions.end());
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            timeBar.Scroll(&window, event);
        }



        window.clear(sf::Color::White);

        int minZ = 99999;
        int maxZ = -99999;
        for (int i = 0; i < drawables.size(); i++) {
            auto val = std::get<1>(drawables[i]);
            minZ = min((int)val, minZ);
            maxZ = max((int)val, maxZ);
        }
        auto d = drawables;
        for (int i = minZ; i <= maxZ; i++) {
            for (int j = 0; j < d.size(); j++) {
                auto z = std::get<1>(d[j]);
                if (z == i) {
                    window.draw(*std::get<0>(d[j]));
                    d.erase(d.begin() + j);
                }
            }
        }

        window.display();
    }
    
    e.CloseReport();
	return 0;
}