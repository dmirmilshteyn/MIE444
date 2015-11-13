#include "Arduino.h"
#include "MappingConstants.h"
#include <LinkedList.h>

#ifndef PATHFINDER_H
#define PATHFINDER_H

struct Position {
  byte x;
  byte y;

  Position() : x(0), y(0) { }
  Position(int x, int y): x(x), y(y) { }
};

struct PathNode {
  PathNode *parent;

  byte x;
  byte y;

  byte g;
  // int h; - always 0 for Dijkstra's algorithm, no need to waste memory for this

  PathNode() : x(0), y(0), g(0), parent(0) { }

  PathNode(int x, int y) : x(x), y(y), g(0), parent(0) { }

  PathNode(int x, int y, int nodeG, PathNode *nodeParent) : x(x), y(y), g(nodeG), parent(nodeParent) { }
};

struct PathfinderResult {
  Position **path;
  int size;

  PathfinderResult(Position **path, int size) : path(path), size(size) { }
};

#define SET_BUFFER_SIZE 50

struct PathNodeSet {
  PathNode* set[SET_BUFFER_SIZE] = { NULL };
  int size;

  PathNodeSet() : size(0) { }

  void add(PathNode *pathNode) {
    for (int i = 0; i < SET_BUFFER_SIZE; i++) {
      if (set[i] == NULL) {
        set[i] = pathNode;
        size++;
        return;
      }
    }
  }

  PathNode* get(int index) {
    return set[index];
  }

  PathNode* remove(int index) {
    PathNode *value = set[index];
    set[index] = NULL;
    size--;

    return value;
  }
};

#define NEIGHBOUR_POSITION_COUNT 8

class Pathfinder {
  public:
    PathfinderResult FindPath(Position startPosition, Position goalPosition);

  private:
    byte *closed_nodes;

    void Initialize();
    void Dispose();

    PathfinderResult ReconstructPath(PathNode *endingNode);
    int DetermineNextNode(LinkedList<PathNode*> *openSet);
    PathfinderResult PerformSearch(Position startPosition, Position goalPosition);
    bool IsBlocked(int x, int y);
    void MarkBlocked(int x, int y, bool value);
    const Position* GetNeighbourPositions(int x, int y);
    int IndexOfSet(LinkedList<PathNode*> *set, int x, int y);

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
