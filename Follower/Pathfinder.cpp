#include <LinkedList.h>

#include "Pathfinder.h"
#include "Mapping.h"

void Pathfinder::Initialize() {
  closed_nodes = new byte[MAP_TILES_WIDTH * MAP_TILES_HEIGHT / 8] { 0 };
}

void Pathfinder::Dispose() {
  delete[] closed_nodes;
}

PathfinderResult Pathfinder::FindPath(Position startPosition, Position goalPosition) {
  Initialize();
  auto path = PerformSearch(startPosition, goalPosition);
  Dispose();

  return path;
}

PathfinderResult Pathfinder::PerformSearch(Position startPosition, Position goalPosition) {
  LinkedList<PathNode> openSet = LinkedList<PathNode>();
  openSet.add(PathNode(startPosition.x, startPosition.y));

  while (openSet.size() > 0) {
    int currentNodeIndex = DetermineNextNode(openSet);
    PathNode currentNode = openSet.get(currentNodeIndex);

    if (currentNode.x == goalPosition.x && currentNode.y == goalPosition.y) {
       return ReconstructPath(currentNode);
    }

    openSet.remove(currentNodeIndex);
    MarkBlocked(currentNode.x, currentNode.y, true);

    const Position* neighbourPositions = GetNeighbourPositions(Position(currentNode.x, currentNode.y));
    
    for (int i = 0; i < NEIGHBOUR_POSITION_COUNT; i++) {
      Position neighbourPosition = neighbourPositions[i];

      if (neighbourPosition.x < 0 || neighbourPosition.y < 0 || neighbourPosition.x > MAP_TILES_WIDTH || neighbourPosition.y > MAP_TILES_HEIGHT) {
        continue;
      }

      // Check if the node has been closed by the pathfinder, or if the node is closed on the map
      if (IsBlocked(neighbourPosition.x, neighbourPosition.y) || // Check if the node has been closed by the pathfinder
          AccessMapElement(neighbourPosition.x, neighbourPosition.y) == false // Check if this location is not a path on the map
          ) {
        continue;
      }

      int tentative_g_score = currentNode.g + CalculateDistanceBetween(Position(currentNode.x, currentNode.y), neighbourPosition);
      int index = IndexOfSet(openSet, Position(currentNode.x, currentNode.y));
      if (index == -1) { // Discover a new node
        openSet.add(PathNode(neighbourPosition.x, neighbourPosition.y, tentative_g_score, &currentNode));
      } else {
        int neighbourIndex = IndexOfSet(openSet, neighbourPosition);
        if (neighbourIndex > -1) {
          auto neighbourNode = openSet.get(neighbourIndex);

          if (tentative_g_score <= neighbourNode.g) {
            neighbourNode.parent = &currentNode;
            neighbourNode.g = tentative_g_score;
          }
        }
      }
    }

    delete[] neighbourPositions;
  }
  
  return PathfinderResult(NULL, 0);
}

PathfinderResult Pathfinder::ReconstructPath(PathNode endingNode) {
  LinkedList<Position> pathList;

  PathNode *currentNode = &endingNode;
  while (currentNode->parent != NULL) {
    pathList.add(Position(currentNode->parent->x, currentNode->parent->y));

    currentNode = currentNode->parent;
  }

  Position *path = new Position[pathList.size()];
  for (int i = pathList.size() - 1, pIndex = 0; i >= 0; i--, pIndex++) {
    path[pIndex] = pathList.get(i);
  }
  return PathfinderResult(path, pathList.size());
}

int Pathfinder::DetermineNextNode(LinkedList<PathNode> openSet) {
  PathNode bestNode;
  int setIndex = -1;

  for (int i = 0; i < openSet.size(); i++) {
    PathNode testNode = openSet.get(i);
    if (setIndex == -1 || testNode.g < testNode.g) {
      bestNode = testNode;
      setIndex = i;
    }
  }
  
  return setIndex;
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

int Pathfinder::IndexOfSet(LinkedList<PathNode> set, Position position) {
  for (int i = 0; i < set.size(); i++) {
    auto node = set.get(i);
    if (node.x == position.x && node.y == position.y) {
      return i;
    }
  }

  return -1;
}

const Position* Pathfinder::GetNeighbourPositions(Position position) {
  Position* neighbourPositions = new Position[8];

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

