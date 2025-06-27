#include "volcan.hpp"
#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
#include "globals.hpp"



const sf::Color BLOQUEADO_COLOR = sf::Color(255, 140, 0);
const int DURACION_BLOQUEO_MS = 3000;
const int INTERVALO_ERUPCION_MS = 10000;

std::vector<CeldaBloqueada> bloqueadas;
sf::Clock relojErupcion;
bool mostrarMensaje = false;
sf::Clock relojMensaje;
bool erupcionInicialHecha = false;

bool estaBloqueada(int id) {
    for (const auto& c : bloqueadas)
        if (c.id == id)
            return true;
    return false;
}

void manejarErupcion(std::map<int, PentTile>& pentagonos, sf::RenderWindow& window, int jugadorId) {
    bool debeEstallar = false;

    

    if (!erupcionInicialHecha) {
        debeEstallar = true;
        erupcionInicialHecha = true;
    } else if (relojErupcion.getElapsedTime().asMilliseconds() > INTERVALO_ERUPCION_MS) {
        debeEstallar = true;
    }

    if (debeEstallar) {
        mostrarMensaje = true;
        relojMensaje.restart();
        std::cout << "\n\t\t\tEl volcan ha estallado!, corre mortal\n\n";

        std::vector<int> ids;
        for (const auto& [id, p] : pentagonos) {
            if (!p.inicio && !p.fin)
                ids.push_back(id);
        }

        std::shuffle(ids.begin(), ids.end(), std::mt19937(std::random_device{}()));
        int num = std::min((int)ids.size(), 30);

        for (int i = 0; i < num; ++i) {
            if (ids[i] == jugadorId)   continue; // no bloquear la celda del jugador

            CeldaBloqueada c;
            c.id = ids[i];
            c.duracion = DURACION_BLOQUEO_MS + rand() % 3000;
            c.reloj.restart();
            bloqueadas.push_back(c);
        }

        relojErupcion.restart();
    }

    if (mostrarMensaje) {
        if (relojMensaje.getElapsedTime().asSeconds() < 2) {
            sf::Font font;
            if (font.loadFromFile("font/MinerCraftory.TTF")) {
                sf::Text texto("¡EL VOLCAN HA ESTALLADO!", font, 36);
                texto.setFillColor(sf::Color::Red);
                texto.setStyle(sf::Text::Bold);
                texto.setPosition((window.getSize().x - texto.getLocalBounds().width) / 2.0f, 20);
                window.draw(texto);
            }
        } else {
            mostrarMensaje = false;
        }
    }

    std::vector<int> desbloquear;
    for (auto& c : bloqueadas) {
        if (c.reloj.getElapsedTime().asMilliseconds() > c.duracion) {
            desbloquear.push_back(c.id);
        }
    }

    for (int id : desbloquear) {
        auto it = std::remove_if(bloqueadas.begin(), bloqueadas.end(), [id](const CeldaBloqueada& c) {
            return c.id == id;
        });
        bloqueadas.erase(it, bloqueadas.end());
        lavaCambio = true;
    }

    for (auto& c : bloqueadas) {
        if (pentagonos.count(c.id)) {
            pentagonos[c.id].shape.setFillColor(BLOQUEADO_COLOR);
        }
    }
}