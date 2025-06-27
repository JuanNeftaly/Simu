#include "slime.hpp"
#include <vector>
#include <random>
#include <iostream>

const int CANTIDAD_SLIMES = 10;

void inicializarSlime(std::map<int, PentTile>& pentagonos) {
    std::vector<int> celdasSlime;
    for (const auto& [id, p] : pentagonos) {
        if (!p.inicio && !p.fin) {
            celdasSlime.push_back(id);
        }
    }

    std::shuffle(celdasSlime.begin(), celdasSlime.end(), std::mt19937(std::random_device{}()));
    for (size_t i = 0; i < CANTIDAD_SLIMES && i < celdasSlime.size(); ++i) {
        pentagonos[celdasSlime[i]].esSlime = true;
    }
}

void manejarSlime(std::map<int, PentTile>& pentagonos, int jugadorId, int turnoActual, bool& atrapado, sf::Clock& relojAtrapado)
{
    if (pentagonos.count(jugadorId) == 0)
    return;

    auto& celda = pentagonos[jugadorId];
    if (celda.esSlime)
    {
        if (turnoActual % 2 == 1)
        {
            atrapado = true;
            relojAtrapado.restart();
            std::cout << "El slime venenoso se encontraba molesto y te ha paralizado!\n";
        }
    }
}
