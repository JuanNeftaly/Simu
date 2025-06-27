#include "mapa_functions.hpp"
#include "mapa_helpers.hpp"
#include "volcan.hpp"
#include "slime.hpp"
#include "terremoto.hpp"
#include "pathfinding.hpp"
#include "globals.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <queue>
#include <random>
#include <limits>
#include <set>

bool inicializarRecursos(sf::Texture &textureLava, sf::Sprite &spriteLava,
                         sf::Texture &textureFloor,
                         sf::Texture &textureHappySlime, sf::Texture &textureAngrySlime,
                         sf::Font &font)
{
    if (!textureLava.loadFromFile(LAVAIMAGE))
        return false;
    spriteLava.setTexture(textureLava);
    spriteLava.setScale(1100.0f / textureLava.getSize().x, 800.0f / textureLava.getSize().y);

    if (!textureFloor.loadFromFile(FLOORIMAGE))
        return false;
    if (!textureHappySlime.loadFromFile(HAPPYSLIMEIMAGE))
        return false;
    if (!textureAngrySlime.loadFromFile(ANGRYSLIMEIMAGE))
        return false;
    if (!font.loadFromFile(GAMEFONT))
        return false;

    return true;
}

void generarMapa(std::map<int, PentTile> &pentagonos, std::map<int, std::vector<int>> &graph, float radius, sf::Texture *textureFloor)
{

    std::vector<std::pair<int, int>> edges;
    if (!readConnections(SOURCECONNECTIONS, edges))
        return;

    float sideLength = 2 * radius * cos(PI / 5);

    size_t maxPentagons = 300;
    int start = edges.front().first;

    std::queue<int> q;
    std::set<int> placed;

    PentTile root;
    root.center = {WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f};
    root.shape = createPentagon(radius, 0.0f, textureFloor);
    root.shape.setPosition(root.center);
    root.id = start;

    q.push(start);
    placed.insert(start);
    pentagonos[start] = root;

    std::map<int, std::vector<int>> tempEdges;
    for (auto [u, v] : edges)
    {
        tempEdges[u].push_back(v);
        tempEdges[v].push_back(u);
    }

    while (!q.empty())
    {
        int current = q.front();
        q.pop();

        if (pentagonos.size() >= maxPentagons)
            break;

        PentTile &base = pentagonos[current];

        for (int neighbor : tempEdges[current])
        {
            if (placed.find(neighbor) == placed.end())
            {
                std::vector<int> sides = {0, 1, 2, 3, 4};
                std::shuffle(sides.begin(), sides.end(), std::mt19937(std::random_device{}()));

                for (int sideBase : sides)
                {
                    if (base.usedEdges[sideBase])
                        continue;

                    int sideNeighbor = (sideBase + 2) % 5;
                    sf::Vector2f edgeCenterBase = getEdgeCenter(base.shape, sideBase);
                    sf::Vector2f unitDir = getUnitVector(base.center, edgeCenterBase);
                    sf::Vector2f newCenter = base.center + unitDir * sideLength;

                    float baseEdgeAngle = 72 * sideBase - 90 + base.rotation;
                    float neighborRotation = baseEdgeAngle + 180 - (72 * sideNeighbor - 90);

                    PentTile vecino;
                    vecino.center = newCenter;
                    vecino.rotation = neighborRotation;
                    vecino.shape = createPentagon(radius, vecino.rotation, textureFloor);
                    vecino.shape.setPosition(vecino.center);
                    vecino.id = neighbor;

                    if (checkOverlap(newCenter, pentagonos, sideLength * 0.95f))
                    {
                        std::cout << "Solapamiento: " << neighbor << "\n";
                        continue;
                    }

                    if (!isWithinWindowBounds(vecino.shape))
                    {
                        std::cout << "Fuera de límites: " << neighbor << "\n";
                        continue;
                    }

                    base.usedEdges[sideBase] = true;
                    vecino.usedEdges[sideNeighbor] = true;

                    pentagonos[neighbor] = vecino;
                    placed.insert(neighbor);
                    q.push(neighbor);

                    graph[current].push_back(neighbor);
                    graph[neighbor].push_back(current);

                    break;
                }
            }
        }
    }

    for (auto &[id1, p1] : pentagonos)
    {
        for (auto &[id2, p2] : pentagonos)
        {
            if (id1 == id2)
                continue;

            for (int i = 0; i < 5; ++i)
            {
                sf::Vector2f a1 = p1.shape.getTransform().transformPoint(p1.shape.getPoint(i));
                sf::Vector2f a2 = p1.shape.getTransform().transformPoint(p1.shape.getPoint((i + 1) % 5));

                for (int j = 0; j < 5; ++j)
                {
                    sf::Vector2f b1 = p2.shape.getTransform().transformPoint(p2.shape.getPoint(j));
                    sf::Vector2f b2 = p2.shape.getTransform().transformPoint(p2.shape.getPoint((j + 1) % 5));

                    float dist1 = std::hypot(a1.x - b2.x, a1.y - b2.y);
                    float dist2 = std::hypot(a2.x - b1.x, a2.y - b1.y);

                    if (dist1 < 2.0f && dist2 < 2.0f)
                    {
                        if (std::find(graph[id1].begin(), graph[id1].end(), id2) == graph[id1].end())
                            graph[id1].push_back(id2);
                        if (std::find(graph[id2].begin(), graph[id2].end(), id1) == graph[id2].end())
                            graph[id2].push_back(id1);
                    }
                }
            }
        }
    }
}

void centrarMapa(std::map<int, PentTile> &pentagonos)
{
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float maxY = std::numeric_limits<float>::lowest();

    for (const auto &[id, p] : pentagonos)
    {
        sf::FloatRect bounds = p.shape.getGlobalBounds();
        minX = std::min(minX, bounds.left);
        minY = std::min(minY, bounds.top);
        maxX = std::max(maxX, bounds.left + bounds.width);
        maxY = std::max(maxY, bounds.top + bounds.height);
    }

    sf::Vector2f offset(
        (WINDOW_WIDTH - (maxX - minX)) / 2.0f - minX,
        (WINDOW_HEIGHT - (maxY - minY)) / 2.0f - minY);

    for (auto &[id, p] : pentagonos)
    {
        p.center += offset;
        p.shape.setPosition(p.center);
    }
}

void inicializarJuego(std::map<int, PentTile> &pentagonos, int &jugadorId, int &finId)
{
    float maxDist = -1.0f;
    int inicioId = -1;
    finId = -1;

    for (auto &[id1, p1] : pentagonos)
    {
        for (auto &[id2, p2] : pentagonos)
        {
            if (id1 == id2)
                continue;
            float dx = p1.center.x - p2.center.x;
            float dy = p1.center.y - p2.center.y;
            float dist = std::sqrt(dx * dx + dy * dy);
            if (dist > maxDist)
            {
                maxDist = dist;
                inicioId = id1;
                finId = id2;
            }
        }
    }

    if (inicioId != -1 && finId != -1)
    {
        pentagonos[inicioId].inicio = true;
        pentagonos[finId].fin = true;
    }

    jugadorId = inicioId;

    inicializarSlime(pentagonos);
    inicializarTerremoto(pentagonos);
}

void procesarInput(sf::RenderWindow &window, sf::Event &event,
                   std::map<int, PentTile> &pentagonos,
                   std::map<int, std::vector<int>> &graph,
                   int &jugadorId, int finId,
                   std::vector<int> &rutaActual, bool &resolviendo, size_t &pasoActual,
                   int &turnoActual, bool &atrapado, sf::Clock &relojAtrapado,
                   const sf::RectangleShape &botonResolver,
                   bool &intentoResolver)

{
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (botonResolver.getGlobalBounds().contains(mousePos))
        {

            intentoResolver = true;

            rutaActual = bfs(jugadorId, finId, graph, bloqueadas);
            if (!rutaActual.empty())
            {
                resolviendo = true;
                pasoActual = 0;
                std::cout << "Ruta encontrada con " << rutaActual.size() - 1 << " pasos.\n";
            }
            else
            {
                std::cout << "No hay ruta disponible (bloqueada por volcán o derrumbe).\n";
                resolviendo = false;
                pasoActual = 0;
            }
        }
        if (atrapado)
        {
            float tiempo_congelar = 5.0f;

            float elapsed = relojAtrapado.getElapsedTime().asSeconds();
            if (elapsed < tiempo_congelar)
                return;
            else
                atrapado = false;
        }

        for (const auto &[id, p] : pentagonos)
        {
            if (p.shape.getGlobalBounds().contains(mousePos))
            {
                auto it = graph.find(jugadorId);
                if (it != graph.end())
                {
                    const std::vector<int> &vecinos = it->second;
                    if (std::find(vecinos.begin(), vecinos.end(), id) != vecinos.end())
                    {
                        bool bloqueada = std::any_of(bloqueadas.begin(), bloqueadas.end(),
                                                     [&](const CeldaBloqueada &c)
                                                     { return c.id == id; });

                        if (!bloqueada)
                        {
                            jugadorId = id;
                            if (pentagonos.count(jugadorId) > 0)
                                manejarSlime(pentagonos, jugadorId, turnoActual, atrapado, relojAtrapado);

                            turnoActual++;

                            if (turnoActual % 10 == 0)
                                ejecutarTerremoto(pentagonos, graph, jugadorId);

                            break;
                        }
                        else
                        {
                            std::cout << "La celda está bloqueada por lava.\n";
                        }
                    }
                }
            }
        }
    }
}

void manejarAutojuego(std::map<int, PentTile> &pentagonos,
                      std::map<int, std::vector<int>> &graph,
                      std::vector<int> &rutaActual, size_t &pasoActual,
                      int &jugadorId, int finId, bool &resolviendo,
                      int &turnoActual, bool &atrapado, sf::Clock &relojAtrapado, sf::Clock &relojPaso)
{
    if (resolviendo && pasoActual < rutaActual.size())
    {
        if (pentagonos.count(jugadorId) == 0)
        {
            std::cout << "El jugador estaba en una celda destruida. Terminando resolución.\n";
            resolviendo = false;
            pasoActual = 0;
            return;
        }

        if (relojPaso.getElapsedTime().asSeconds() >= 0.5f)
        {
            if (pentagonos.count(rutaActual[pasoActual]) == 0)
            {
                std::cout << "El siguiente paso ya no existe (derrumbado). Recalculando...\n";
                rutaActual = bfs(jugadorId, finId, graph, bloqueadas);
                pasoActual = 0;

                if (rutaActual.empty())
                {
                    std::cout << "Ya no hay ruta disponible!\n";
                    resolviendo = false;
                    pasoActual = 0;
                    return;
                }

                relojPaso.restart();
                return;
            }

            bool bloqueada = std::any_of(bloqueadas.begin(), bloqueadas.end(),
                                         [&](const CeldaBloqueada &c)
                                         { return c.id == rutaActual[pasoActual]; });

            if (bloqueada)
            {
                std::cout << "Recalculando ruta por obstáculo en tiempo real...\n";
                rutaActual = bfs(jugadorId, finId, graph, bloqueadas);
                pasoActual = 0;

                if (rutaActual.empty())
                {
                    std::cout << "Ya no hay ruta disponible!\n";
                    resolviendo = false;
                    pasoActual = 0;
                    return;
                }

                relojPaso.restart();
                return;
            }

            if (atrapado)
            {
                float elapsed = relojAtrapado.getElapsedTime().asSeconds();
                if (elapsed < 5.0f)
                    return;
                else
                    atrapado = false;
            }

            if (jugadorId != rutaActual[pasoActual])
            {
                jugadorId = rutaActual[pasoActual];

                if (pentagonos.count(jugadorId) > 0)
                    manejarSlime(pentagonos, jugadorId, turnoActual, atrapado, relojAtrapado);

                turnoActual++;
                if (turnoActual % 10 == 0)
                    ejecutarTerremoto(pentagonos, graph, jugadorId);
            }

            pasoActual++;

            if (pasoActual >= rutaActual.size())
            {
                resolviendo = false;
                std::cout << "¡Ruta completada!\n";
            }

            relojPaso.restart();
        }
    }
}

void dibujar(sf::RenderWindow &window, std::map<int, PentTile> &pentagonos,
             int jugadorId, int turnoActual,
             const sf::Texture &textureAngrySlime, const sf::Texture &textureHappySlime,
             const sf::Sprite &spriteLava, const sf::Font &font,
             const sf::RectangleShape &botonResolver)
{
    window.clear();
    window.draw(spriteLava);

    for (auto &[id, p] : pentagonos)
    {
        bool estaBloqueada = std::any_of(bloqueadas.begin(), bloqueadas.end(),
                                         [&](const CeldaBloqueada &c)
                                         { return c.id == id; });

        if (estaBloqueada)
        {
            p.shape.setFillColor(sf::Color(255, 140, 0));
            p.shape.setOutlineColor(sf::Color::Black);
            p.shape.setOutlineThickness(3);
        }
        else if (id == jugadorId)
        {
            p.shape.setFillColor(sf::Color::Yellow);
            p.shape.setOutlineColor(sf::Color::Red);
            p.shape.setOutlineThickness(5);
        }
        else if (p.inicio)
        {
            p.shape.setOutlineColor(sf::Color::Green);
            p.shape.setFillColor(sf::Color::Green);
            p.shape.setOutlineThickness(5);
        }
        else if (p.fin)
        {
            p.shape.setOutlineColor(sf::Color::Blue);
            p.shape.setFillColor(sf::Color::Blue);
            p.shape.setOutlineThickness(5);
        }
        else if (p.esSlime)
        {
            if (turnoActual % 2 == 1)
            {
                p.shape.setTexture(&textureAngrySlime);
                p.shape.setFillColor(sf::Color(255, 255, 255));
            }
            else
            {
                p.shape.setTexture(&textureHappySlime);
                p.shape.setFillColor(sf::Color(255, 255, 255));
            }

            p.shape.setOutlineColor(sf::Color::Magenta);
            p.shape.setOutlineThickness(2);
        }
        else
        {
            p.shape.setFillColor(sf::Color::White);
            p.shape.setOutlineColor(sf::Color::White);
            p.shape.setOutlineThickness(2);
        }

        window.draw(p.shape);
    }

    window.draw(botonResolver);

    sf::Text texto("Resolver", font, 20);
    texto.setFillColor(sf::Color::White);
    texto.setPosition(botonResolver.getPosition().x + 20, botonResolver.getPosition().y + 15);
    window.draw(texto);

    sf::Text textoTurno;
    textoTurno.setFont(font);
    textoTurno.setString("Turno " + std::to_string(turnoActual));
    textoTurno.setCharacterSize(24);
    textoTurno.setFillColor(sf::Color::White);
    textoTurno.setPosition(20, 20);
    window.draw(textoTurno);

    window.display();
}
