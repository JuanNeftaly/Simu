#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include <string>
#include "mapa.hpp"

sf::ConvexShape createPentagon(float radius, float rotation, sf::Texture *texture);
sf::Vector2f getUnitVector(sf::Vector2f from, sf::Vector2f to);
bool checkOverlap(const sf::Vector2f &newCenter, const std::map<int, PentTile> &pentagonos, float minDist);
bool isWithinWindowBounds(const sf::ConvexShape &pentagon);
sf::Vector2f getEdgeCenter(const sf::ConvexShape &pent, int edgeIndex);

bool readConnections(const std::string &filename, std::vector<std::pair<int, int>> &edges);
