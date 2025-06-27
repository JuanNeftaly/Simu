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

```cpp
while (current != startId) {
    path.push_back(current);
    current = came_from[current];
}
path.push_back(startId);
std::reverse(path.begin(), path.end());
```
