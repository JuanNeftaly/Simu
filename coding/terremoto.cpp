#include "terremoto.hpp"
#include <random>
#include <iostream>

std::vector<int> derrumbables;
std::vector<int> derrumbadas;

void inicializarTerremoto(std::map<int, PentTile> &pentagonos)
{
    derrumbables.clear();
    derrumbadas.clear();

    for (const auto &[id, p] : pentagonos)
    {
        if (!p.inicio && !p.fin)
            derrumbables.push_back(id);
    }

    std::shuffle(derrumbables.begin(), derrumbables.end(), std::mt19937(std::random_device{}()));
}

void ejecutarTerremoto(std::map<int, PentTile>& pentagonos, std::map<int, std::vector<int>>& graph, int jugadorId)
{
    int cantidad = 10; 
    int derrumbadasAhora = 0;

    while (!derrumbables.empty() && derrumbadasAhora < cantidad)
    {
        int id = derrumbables.back();
        derrumbables.pop_back();

        if (id == jugadorId)
            continue; 

        derrumbadas.push_back(id);

        pentagonos.erase(id);
        graph.erase(id);

        for (auto& [nid, vecinos] : graph)
        {
            vecinos.erase(std::remove(vecinos.begin(), vecinos.end(), id), vecinos.end());
        }

        std::cout << "Terremoto! La celda " << id << " se ha derrumbado.\n";

        derrumbadasAhora++;
    }
}



bool esDerrumbada(int id)
{
    return std::find(derrumbadas.begin(), derrumbadas.end(), id) != derrumbadas.end();
}