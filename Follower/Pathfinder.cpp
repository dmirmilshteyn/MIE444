#include "Pathfinder.h"

void Pathfinder::Initialize() {
  closed_nodes = new byte[MAP_TILES_WIDTH * MAP_TILES_HEIGHT / 8] { 0 };
}

void Pathfinder::Dispose() {
  delete[] closed_nodes;
}

bool Pathfinder::IsBlocked(int x, int y) {
  int position = CalculateInternalPosition(x, y);
  int offset = CalculateInternalOffset(x, y);

  byte internal_value = closed_nodes[position];

  return internal_value & (1 << offset);
}

void Pathfinder::MarkBlocked(int x, int y, bool value) {
  int position = CalculateInternalPosition(x, y);
  int offset = CalculateInternalOffset(x, y);

  byte internal_value = closed_nodes[position];

  if (value) {
    internal_value = internal_value | 1 << offset;
  } else {
    internal_value = internal_value & ~(1 << offset);
  }

  closed_nodes[position] = internal_value;
}

const Position* Pathfinder::GetNeighbourPositions(Position position) {
  Position *neighbourPositions = new Position[8];

  // Top row
  neighbourPositions[0] = Position(position.x - 1, position.y - 1);
  neighbourPositions[1] = Position(position.x, position.y - 1);
  neighbourPositions[2] = Position(position.x + 1, position.y - 1);
  // Middle row
  neighbourPositions[3] = Position(position.x - 1, position.y);
  neighbourPositions[4] = Position(position.x + 1, position.y);
  // Bottom row
  neighbourPositions[5] = Position(position.x - 1, position.y + 1);
  neighbourPositions[6] = Position(position.x, position.y + 1);
  neighbourPositions[7] = Position(position.x + 1, position.y + 1);

  return neighbourPositions;
}

