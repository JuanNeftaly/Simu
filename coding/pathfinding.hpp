#pragma once
#include <vector>
#include <map>
#include "volcan.hpp"

std::vector<int> bfs(int startId, int goalId, const std::map<int, std::vector<int>>& graph, const std::vector<CeldaBloqueada>& bloqueadas);