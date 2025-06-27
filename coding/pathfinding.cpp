#include "pathfinding.hpp"
#include <queue>
#include <set>
#include <algorithm>

std::vector<int> bfs(int startId, int goalId, const std::map<int, std::vector<int>>& graph, const std::vector<CeldaBloqueada>& bloqueadas) {
    std::queue<int> q;
    std::map<int, int> came_from;
    std::set<int> visited;

    q.push(startId);
    visited.insert(startId);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        if (current == goalId) {
            // Reconstruir camino
            std::vector<int> path;
            while (current != startId) {
                path.push_back(current);
                current = came_from[current];
            }
            path.push_back(startId);
            std::reverse(path.begin(), path.end());
            return path;
        }

        auto it = graph.find(current);
        if (it == graph.end()) continue;

        for (int neighbor : it->second) {
            bool bloqueada = std::any_of(bloqueadas.begin(), bloqueadas.end(), [&](const CeldaBloqueada &c)
            { return c.id == neighbor; });

            if (!bloqueada && visited.find(neighbor) == visited.end()) {
                q.push(neighbor);
                visited.insert(neighbor);
                came_from[neighbor] = current;
            }
        }
    }

    // no se encontro camino
    return {};
}