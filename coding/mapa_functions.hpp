#pragma once
#include <SFML/Graphics.hpp>
#include "mapa.hpp"


bool inicializarRecursos(sf::Texture& textureLava, sf::Sprite& spriteLava,
                         sf::Texture& textureFloor,
                         sf::Texture& textureHappySlime, sf::Texture& textureAngrySlime,
                         sf::Font& font);

void generarMapa(std::map<int, PentTile>& pentagonos, std::map<int, std::vector<int>>& graph, float radius, sf::Texture* textureFloor);

void centrarMapa(std::map<int, PentTile>& pentagonos);

void inicializarJuego(std::map<int, PentTile>& pentagonos, int& jugadorId, int& finId);

void procesarInput(sf::RenderWindow &window, sf::Event &event,
                   std::map<int, PentTile> &pentagonos,
                   std::map<int, std::vector<int>> &graph,
                   int &jugadorId, int finId,
                   std::vector<int> &rutaActual, bool &resolviendo, size_t &pasoActual,
                   int &turnoActual, bool &atrapado, sf::Clock &relojAtrapado,
                   const sf::RectangleShape &botonResolver,
                   bool& intentoResolver
                );


void manejarAutojuego(std::map<int, PentTile>& pentagonos,
                      std::map<int, std::vector<int>>& graph,
                      std::vector<int>& rutaActual, size_t& pasoActual,
                      int& jugadorId, int finId, bool& resolviendo,
                      int& turnoActual, bool& atrapado, sf::Clock& relojAtrapado, sf::Clock& relojPaso);

void dibujar(sf::RenderWindow& window, std::map<int, PentTile>& pentagonos,
             int jugadorId, int turnoActual,
             const sf::Texture& textureAngrySlime, const sf::Texture& textureHappySlime,
             const sf::Sprite& spriteLava, const sf::Font& font,
             const sf::RectangleShape& botonResolver);
