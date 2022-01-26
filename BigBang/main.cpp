#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <format>
#include <random>

#include "Particle.h"
#include "TimeBar.h"
#include "Error.h"
#include "BigBang.h"

using namespace std;

enum class ParticleType {
    Quark = 5,
    Electron = 5,
    Atom = 5,
};

ErrorReport e = ErrorReport("bigbang program error.txt");
sf::Font mainFont;

sf::Text newText(string text) {
    sf::Text t = sf::Text(text, mainFont, 20U);
    t.setFillColor(sf::Color::White);
    return t;
}
float randomRange(int x, int x2) {
    static std::default_random_engine e;
    static std::uniform_real_distribution<> dis(x, x2); // rage 0 - 1
    return dis(e);
}
void display(sf::RenderWindow* window, vector<tuple<sf::Drawable*, unsigned int>>* drawables) {
    int minZ = 99999;
    int maxZ = -99999;
    for (int i = 0; i < (*drawables).size(); i++) {
        auto val = std::get<1>((*drawables)[i]);
        minZ = min((int)val, minZ);
        maxZ = max((int)val, maxZ);
    }
    auto d = *drawables;
    for (int i = minZ; i <= maxZ; i++) {
        for (int j = 0; j < d.size(); j++) {
            auto z = std::get<1>(d[j]);
            if (z == i) {
                (*window).draw(*std::get<0>(d[j]));
            }
        }
    }
}

int main() {
    //pf
    if (!mainFont.loadFromFile("NotoSansKR-Medium.otf")) {
        e.WriteLine("Failed to load font NotoSansKR-Medium.otf");
        return 0;
    }

    auto windowVideoMode = sf::VideoMode(1000, 800);
    sf::RenderWindow window(windowVideoMode, "Big Bang", sf::Style::Titlebar | sf::Style::Close);
    window.setKeyRepeatEnabled(false);

    vector<tuple<sf::Drawable*, unsigned int>> drawables = vector<tuple<sf::Drawable*, unsigned int>>();

    const int particleN = 5;
    const int r = 1;
    TimeBar timeBar = TimeBar(windowVideoMode, 40, 100);
    sf::Text timeIndicator = newText("0");
    
    auto compositions = timeBar.GetCompositions();
    drawables.insert(drawables.end(), compositions.begin(), compositions.end());
    drawables.push_back(make_tuple(&timeIndicator, 1U));

    BigBang bigbang = BigBang(sf::Vector2f(windowVideoMode.width / 2, windowVideoMode.height / 2));
    drawables.push_back(make_tuple(&bigbang, 0U));
    vector<QuarkGroup> proton = vector<QuarkGroup>();
    vector<QuarkGroup> neutron = vector<QuarkGroup>();
    vector<Particle> electron = vector<Particle>();
    //vector<Particle> atom = vector<Particle>();

    
    for (int i = 0; i < particleN; i++) {
        auto p = QuarkGroup(Nucleus::Proton, (int)ParticleType::Quark, randomRange(-r, r));
        auto n = QuarkGroup(Nucleus::Neutron, (int)ParticleType::Quark, randomRange(-r, r));
        auto e = Particle("Electron", (int)ParticleType::Electron, randomRange(-r, r));
        p.SetBackgroundColor(sf::Color::Yellow);
        n.SetBackgroundColor(sf::Color::Green);
        e.SetBackgroundColor(sf::Color::Cyan);

        proton.push_back(p);
        neutron.push_back(n);
        electron.push_back(e);
    }
    for (int i = 0; i < particleN; i++) {
        bigbang.AddQuark(&proton[i]);
        bigbang.AddQuark(&neutron[i]);
        bigbang.AddParticle(&electron[i]);
    }

    int currentTime = 0;
    int k = 30;
    int quarkStart = 50; 

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            timeBar.Scroll(&window, event);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                timeBar.Plus(k * -1);
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                timeBar.Plus(k);
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                for (int i = 0; i < particleN; i++) {
                    neutron[i].AddParticle();
                    proton[i].AddParticle();
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                for (int i = 0; i < particleN; i++) {
                    neutron[i].PopParticle();
                    proton[i].PopParticle();
                }
            }
        }

        int current = timeBar.Current();
        
        currentTime = current;

        timeIndicator.setPosition(windowVideoMode.width / 2 - timeIndicator.getLocalBounds().width / 2, 10);
        timeIndicator.setString("After " + std::to_string(timeBar.Current()) + " Seconds");
        bigbang.Expand(current+1);

        window.clear(sf::Color::Black);
        
        display(&window, &drawables);

        window.display();
    }
    
    e.CloseReport();
	return 0;
}