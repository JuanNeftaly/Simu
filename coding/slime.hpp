#pragma once
#include "mapa.hpp"
#include <map>

void inicializarSlime(std::map<int, PentTile>& pentagonos);
void manejarSlime(std::map<int, PentTile>& pentagonos, int jugadorId, int turnoActual, bool& atrapado, sf::Clock& relojAtrapado);