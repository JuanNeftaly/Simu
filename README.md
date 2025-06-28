# Escape the Volcan 🌋

## Algoritmos utilizados y su Implementacion

### Búsqueda en Anchura (Breadth-First Search, BFS)

A continuación se documenta el algoritmo de búsqueda en anchura (BFS) que emplea la aplicación para trazar la ruta más corta entre el punto de partida y la meta, respetando que hay celdas que pueden tanto bloquearse como desbloquearse con lava.

El Algoritmo BFS tiene como objetivo para este caso hallar el camino mas corot en **un grafo no ponderado** que modela la cuadricula pentagonal

### Representación del grafo

- **Nodos:** Cada celda del mapa tiene un identificador único (`int`).
- **Aristas:** Se representan como un `std::map<int, std::vector<int>>`, donde cada nodo está asociado a sus vecinos accesibles.
- **Bloqueo dinámico:** Las celdas bloqueadas por lava se representan como una lista de objetos `CeldaBloqueada`, y se filtran antes de visitar los vecinos.

A continuacion se muestra la funcion que nos ayuda a implementar el Algoritmo BFS.

```cpp
std::vector<int> bfs(
    int startId,
    int goalId,
    const std::map<int, std::vector<int>>& graph,
    const std::vector<CeldaBloqueada>& bloqueadas
);
```

1. Se inicializa con una cola `std::queue` para gestionar los nodos pendientes de explorar

2. Se lleva un registro de nodos visitados en `std::set` y de la procedencia de cada nodo en `std::map`

3. Mientas haya nodos en al cola hace lo siguiente:

    - procesa el nodo actual

    - si es la meta, se reconstruye el camino usando el mapa `came_from`

    - se exploran sus vecinos, verificando que no estan ni bloqueados, ni vacios

4. Si no se encuentra ruta, se devuelve un vector vacio.

Una implementacion clave es la reconstruccion del ruta, la cual constantemente esta evaluando a medida avanzamos

```cpp
while (current != startId) {
    path.push_back(current);
    current = came_from[current];
}
path.push_back(startId);
std::reverse(path.begin(), path.end());
```

## Biblioteca SMFL

SFML (*Simple and Fast Multimedia Library*) es una biblioteca gráfica y multimedia desarrollada en C++ que proporciona una interfaz simple para programar aplicaciones gráficas 2D, videojuegos, simulaciones y sistemas interactivos. Fue diseñada para ser rápida, portátil y fácil de integrar en proyectos C++ modernos.

## Características de SFML

- **Modularidad:** Está dividida en varios módulos:
  - `Graphics`: renderizado de formas, sprites, texto, etc.
  - `Window`: gestión de la ventana, eventos, entrada de teclado y ratón.
  - `System`: manejo de tiempo, hilos, vectores y utilidades básicas.
  - `Audio`: reproducción de sonido y música.
  - `Network`: soporte para sockets y comunicaciones de red.
- **Multiplataforma:** Funciona en Windows, Linux y macOS.
- **Fácil integración:** Compatible con herramientas modernas de desarrollo como CMake, y entornos como Visual Studio, Code::Blocks, etc.
- **Ligera y eficiente:** Está pensada para gráficos 2D en tiempo real.

SFML fue elegida tambien por su soporte directo para dibujar formas personalizadas, la cual nos fue util para los pentagonos solicitados, asi como capturar eventos del ratón y renderizar de manera dinamica cada frame dentro del juego.

## Manual para uso de SMFL

A continuacion se muestra la implementacion basica de SMFL

```cpp
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Escape the Grid");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();           // Limpia la ventana
        
        ... aqui es donde se dibujan los elementos 

        window.display();         // Muestra lo dibujado
    }

    return 0;
}
```

## Manejo de la visualizacion

El juego consta de diferentes vistas, un menu de inicio, el tablero del juego y la finalizacion, se maneja de la siguiente manera:

Todo comienza en nuestro archivo `main.cpp` el cual solo incluye el menu para iniciar el juego.

```cpp
#include "menu.hpp"

int main() {
    iniciarMenu();
    return 0;
}
```

Como se puede ver se manda a llamar una funcion `iniciarMenu()` la cual contiene lo primero que visualiza el usuario al momento de entrar al juego
