#include "mapa_helpers.hpp"
#include <fstream>
#include <cmath>
#include <algorithm>
#include "globals.hpp"


sf::ConvexShape createPentagon(float radius, float rotation, sf::Texture* texture)
{
    sf::ConvexShape pentagon;
    pentagon.setPointCount(5);
    for (int i = 0; i < 5; ++i)
    {
        float angle_deg = 72 * i - 90 + rotation;
        float angle_rad = angle_deg * PI / 180.0f;
        pentagon.setPoint(i, sf::Vector2f(
                                 radius * cos(angle_rad),
                                 radius * sin(angle_rad)));
    }
    if (texture)
    {
        pentagon.setTexture(texture);
        sf::IntRect textureRect(0, 0, texture->getSize().x, texture->getSize().y);
        pentagon.setTextureRect(textureRect);
    }
    return pentagon;
}

sf::Vector2f getEdgeCenter(const sf::ConvexShape &pent, int edgeIndex)
{
    sf::Vector2f a = pent.getTransform().transformPoint(pent.getPoint(edgeIndex));
    sf::Vector2f b = pent.getTransform().transformPoint(pent.getPoint((edgeIndex + 1) % 5));
    return (a + b) / 2.0f;
}

sf::Vector2f getUnitVector(sf::Vector2f from, sf::Vector2f to)
{
    sf::Vector2f dir = to - from;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len == 0)
        return sf::Vector2f(0, 0);
    return dir / len;
}

bool checkOverlap(const sf::Vector2f &newCenter, const std::map<int, PentTile> &pentagonos, float minDist)
{
    for (const auto &[id, p] : pentagonos)
    {
        sf::Vector2f diff = p.center - newCenter;
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        if (dist < minDist)
            return true;
    }
    return false;
}

bool isWithinWindowBounds(const sf::ConvexShape &pentagon)
{
    for (int i = 0; i < 5; ++i)
    {
        sf::Vector2f pt = pentagon.getTransform().transformPoint(pentagon.getPoint(i));
        if (pt.x < 0 || pt.x >= WINDOW_WIDTH ||
            pt.y < 0 || pt.y >= WINDOW_HEIGHT)
            return false;
    }
    return true;
}

bool readConnections(const std::string &filename, std::vector<std::pair<int, int>> &edges)
{
    std::ifstream file(filename);
    if (!file.is_open())
        return false;

    int a, b;
    while (file >> a >> b)
    {
        if (a < b)
            edges.push_back({a, b});
    }
    return true;
}
