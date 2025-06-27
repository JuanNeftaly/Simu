#include <SFML/Graphics.hpp>
#include <iostream>
#include "mapa.hpp"
#include "menu.hpp"
#include "globals.hpp"


void iniciarMenu()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Volcarinto Menu");

    sf::Texture fondoTexture;
    if (!fondoTexture.loadFromFile(VOLCANIMAGE)) {
        std::cerr << "No se pudo cargar volcan_menu.png\n";
        return;
    }

    sf::Sprite fondo(fondoTexture);
    fondo.setScale(
        float(WINDOW_WIDTH) / fondoTexture.getSize().x,
        float(WINDOW_HEIGHT) / fondoTexture.getSize().y
    );

    sf::RectangleShape boton(sf::Vector2f(200, 60));
    boton.setFillColor(sf::Color(70, 70, 70));
    boton.setPosition((WINDOW_WIDTH - 200) / 2.0f, (WINDOW_HEIGHT - 60) / 2.0f);

    sf::Font font;
    if (!font.loadFromFile(GAMEFONT));

    sf::Text textoBoton("JUGAR", font, 26);
    textoBoton.setFillColor(sf::Color::White);
    textoBoton.setStyle(sf::Text::Bold);
    textoBoton.setPosition(
        boton.getPosition().x + (boton.getSize().x - textoBoton.getLocalBounds().width) / 2.0f,
        boton.getPosition().y + (boton.getSize().y - textoBoton.getLocalBounds().height) / 2.5f
    );

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mouse(sf::Mouse::getPosition(window));
                if (boton.getGlobalBounds().contains(mouse))
                {
                    window.close();
                    iniciarMapa();
                }
            }
        }

        if (boton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
        {
            boton.setFillColor(sf::Color(100, 100, 100));
        }
        else
        {
            boton.setFillColor(sf::Color(70, 70, 70));
        }

        window.clear();
        window.draw(fondo);
        window.draw(boton);
        window.draw(textoBoton);
        window.display();
    }
}
