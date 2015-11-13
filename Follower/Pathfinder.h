#include "Arduino.h"
#include "MappingConstants.h"
#include <LinkedList.h>

#ifndef PATHFINDER_H
#define PATHFINDER_H

struct Position {
  int x;
  int y;

  Position() { }
  Position(int x, int y): x(x), y(y) { }
};

struct PathNode {
  PathNode *parent;

  int x;
  int y;

  int g;
  // int h; - always 0 for Dijkstra's algorithm, no need to waste memory for this

  PathNode() {
    x = -1;
    y = -1;
  }

  PathNode(int x, int y) : x(x), y(y) { }

  PathNode(int x, int y, int nodeG, PathNode *nodeParent) {
    x = x;
    y = y;
    g = nodeG;
    parent = nodeParent;
  }
};

struct PathfinderResult {
  Position *path;
  int size;

  PathfinderResult(Position *path, int size) : path(path), size(size) { }
};

#define NEIGHBOUR_POSITION_COUNT 8

class Pathfinder {
  public:
    PathfinderResult FindPath(Position startPosition, Position goalPosition);

  private:
    byte *closed_nodes;

    void Initialize();
    void Dispose();

    PathfinderResult ReconstructPath(PathNode endingNode);
    int DetermineNextNode(LinkedList<PathNode> openSet);
    PathfinderResult PerformSearch(Position startPosition, Position goalPosition);
    bool IsBlocked(int x, int y);
    void MarkBlocked(int x, int y, bool value);
    const Position* GetNeighbourPositions(Position position);
    int IndexOfSet(LinkedList<PathNode> set, Position position);

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
