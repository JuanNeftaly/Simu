#pragma once
#include "mapa.hpp"
#include <map>
#include <vector>

void inicializarTerremoto(std::map<int, PentTile>& pentagonos);
void ejecutarTerremoto(std::map<int, PentTile>& pentagonos, std::map<int, std::vector<int>>& graph, int jugadorId);
bool esDerrumbada(int id);