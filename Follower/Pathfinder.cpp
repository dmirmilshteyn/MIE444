#include "Pathfinder.h"

void Pathfinder::Initialize() {
  closed_nodes = new byte[MAP_TILES_WIDTH * MAP_TILES_HEIGHT / 8] { 0 };
}

void Pathfinder::Dispose() {
  delete[] closed_nodes;
}

float Pathfinder::CalculateDistanceBetween(Node nodeA, Node nodeB) {
  return 0;
}
