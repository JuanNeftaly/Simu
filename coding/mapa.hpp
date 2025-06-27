#pragma once
#include <SFML/Graphics.hpp>
#include <vector>



struct PentTile {
    sf::ConvexShape shape;
    sf::Vector2f center;
    float rotation = 0.0f;
    std::vector<bool> usedEdges = std::vector<bool>(5, false);
    int id = -1;
    bool inicio = false;
    bool fin = false;
    bool esSlime = false;
    
};


void iniciarMapa();