#include "mapa_functions.hpp"
#include "globals.hpp"
#include "mapa_helpers.hpp"
#include <iostream>
#include "pathfinding.hpp"



void iniciarMapa()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Volcarinto");

    sf::Texture textureLava, textureFloor, textureHappySlime, textureAngrySlime;
    sf::Sprite spriteLava;
    sf::Font font;

    if (!inicializarRecursos(textureLava, spriteLava, textureFloor, textureHappySlime, textureAngrySlime, font))
        return;

    std::map<int, PentTile> pentagonos;
    std::map<int, std::vector<int>> graph;

    float radius = 40.0f;
    generarMapa(pentagonos, graph, radius, &textureFloor);
    centrarMapa(pentagonos);

    int jugadorId = -1;
    int finId = -1;
    inicializarJuego(pentagonos, jugadorId, finId);

    sf::RectangleShape botonResolver(sf::Vector2f(150, 50));
    botonResolver.setPosition(WINDOW_WIDTH - 170, WINDOW_HEIGHT - 70);
    botonResolver.setFillColor(sf::Color(100, 100, 100));

    std::vector<int> rutaActual;
    bool resolviendo = false;
    size_t pasoActual = 0;
    sf::Clock relojPaso;
    

    sf::Clock relojAtrapado;
    bool atrapado = false;
    bool intentoResolver = false;

    int turnoActual = 0;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            procesarInput(window, event, pentagonos, graph, jugadorId, finId,
                          rutaActual, resolviendo, pasoActual, turnoActual,
                          atrapado, relojAtrapado, botonResolver, intentoResolver);
        }

        manejarErupcion(pentagonos, window, jugadorId);

        if (lavaCambio && intentoResolver)
        {
            std::cout << "La lava ha cambiado. Recalculando ruta...\n";
            rutaActual = bfs(jugadorId, finId, graph, bloqueadas);
            pasoActual = 0;

            if (rutaActual.empty())
            {
                std::cout << "Todavía no hay ruta disponible.\n";
                resolviendo = false;
            }
            else
            {
                std::cout << "Nueva ruta encontrada tras cambio de lava.\n";
                resolviendo = true;
            }

            lavaCambio = false;
        }

        manejarAutojuego(pentagonos, graph, rutaActual, pasoActual,
                         jugadorId, finId, resolviendo, turnoActual,
                         atrapado, relojAtrapado, relojPaso);

        dibujar(window, pentagonos, jugadorId, turnoActual,
                textureAngrySlime, textureHappySlime, spriteLava,
                font, botonResolver);
    }
}
