#include "Arduino.h"
#include "MappingConstants.h"
#include <LinkedList.h>

#ifndef PATHFINDER_H
#define PATHFINDER_H

struct Position {
  int x;
  int y;

  Position(int x, int y): x(x), y(y) { }
  Position();
};

struct PathNode {
  PathNode *parent;

  Position position;

  int g;
  // int h; - always 0 for Dijkstra's algorithm, no need to waste memory for this
};

class Pathfinder {
  public:
    Position* FindPath(Position startPosition, Position goalPosition);

  private:
    byte *closed_nodes;

    void Initialize();
    void Dispose();

    PathNode DetermineNextNode(LinkedList<PathNode> openSet);
    Position* PerformSearch(Position startPosition, Position goalPosition);
    bool IsBlocked(int x, int y);
    void MarkBlocked(int x, int y, bool value);
    const Position* GetNeighbourPositions(Position position);

    inline float CalculateDistanceBetween(Position positionA, Position positionB) {
      return pow(positionB.x - positionA.x, 2) + pow(positionB.y - positionA.y, 2);
    }

    inline int CalculateInternalPosition(int x, int y) {
      return (y * MAP_TILES_WIDTH + x) / 8;
    }

    inline int CalculateInternalOffset(int x, int y) {
      return (y * MAP_TILES_WIDTH + x) % 8;
    }
};

#endif
