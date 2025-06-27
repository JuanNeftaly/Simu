#pragma once
#include <map>
#include <vector>
#include <SFML/Graphics.hpp>
#include "mapa.hpp"

struct CeldaBloqueada {
    int id;
    sf::Clock reloj;
    int duracion;
};

extern std::vector<CeldaBloqueada> bloqueadas;

void manejarErupcion(std::map<int, PentTile>& pentagonos, sf::RenderWindow& window, int jugadorId);
bool estaBloqueada(int id);