#include "Pathfinder.h"
#include "Mapping.h"
#include "MemoryFree.h"
#include <LinkedList.h>

void Pathfinder::Initialize() {
  closed_nodes = new byte[MAP_TILES_WIDTH * MAP_TILES_HEIGHT / 8]();
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
  LinkedList<PathNode*> *openSet = new LinkedList<PathNode*>();

  println(sizeof(ListNode<PathNode*>));
  int nodeCount = 1;

  print("Free Memory: ");
  println(freeMemory());

  // Add the starting position
  openSet->add(new PathNode(startPosition.x, startPosition.y));

  while (openSet->size() > 0) {
    print("Free Memory: ");
    print(freeMemory());
    print(", Node Count: ");
    println(nodeCount);
    
    int currentNodeIndex = DetermineNextNode(openSet);
    PathNode *currentNode = openSet->remove(currentNodeIndex);

    if (currentNode->x == goalPosition.x && currentNode->y == goalPosition.y) {
      return ReconstructPath(currentNode);
    }

    MarkBlocked(currentNode->x, currentNode->y, true);

    const Position* neighbourPositions = GetNeighbourPositions(currentNode->x, currentNode->y);

    bool stillAlive = false;
    for (int i = 0; i < NEIGHBOUR_POSITION_COUNT; i++) {
      Position neighbourPosition = neighbourPositions[i];

      if (neighbourPosition.x < 0 || neighbourPosition.y < 0 || neighbourPosition.x >= MAP_TILES_WIDTH || neighbourPosition.y >= MAP_TILES_HEIGHT) {
        continue;
      }

      // Check if the node has been closed by the pathfinder, or if the node is closed on the map
      if (IsBlocked(neighbourPosition.x, neighbourPosition.y) || // Check if the node has been closed by the pathfinder
          AccessMapElement(neighbourPosition.x, neighbourPosition.y) == false // Check if this location is not a path on the map
         ) {
        continue;
      }

      byte tentative_g_score = currentNode->g + CalculateDistanceBetween(Position(currentNode->x, currentNode->y), neighbourPosition);
      int index = IndexOfSet(openSet, currentNode->x, currentNode->y);
      if (index == -1) { // Discover a new node
        PathNode *setNode = new PathNode(neighbourPosition.x, neighbourPosition.y, tentative_g_score, currentNode);
        openSet->add(setNode);
        stillAlive = true;
        
        nodeCount++;
      } else {
        int neighbourIndex = IndexOfSet(openSet, neighbourPosition.x, neighbourPosition.y);
        if (neighbourIndex > -1) {
          auto neighbourNode = openSet->get(neighbourIndex);

          if (tentative_g_score <= neighbourNode->g) {
            neighbourNode->parent = currentNode;
            neighbourNode->g = tentative_g_score;
            stillAlive = true;
          }
        }
      }
    }

    if (!stillAlive) {
      delete currentNode;
      nodeCount--;
    }
    delete[] neighbourPositions;
  }

  return PathfinderResult(NULL, 0);
}

PathfinderResult Pathfinder::ReconstructPath(PathNode *endingNode) {
  int depth = 0;
  PathNode *currentNode = endingNode;
  while (currentNode != NULL) {
    depth++;
    currentNode = currentNode->parent;
  }

  currentNode = endingNode;
  
  Position **path = new Position*[depth]();
  for (int i = depth - 1; i >= 0; i--) {
    path[i] = new Position(currentNode->x, currentNode->y);

    currentNode = currentNode->parent;
  }
  return PathfinderResult(path, depth);
}

int Pathfinder::DetermineNextNode(LinkedList<PathNode*> *openSet) {
  PathNode* bestNode = new PathNode();
  int setIndex = -1;

  for (int i = 0; i < openSet->size(); i++) {
    PathNode* testNode = openSet->get(i);
    if (setIndex == -1 || testNode->g < bestNode->g) {
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

int Pathfinder::IndexOfSet(LinkedList<PathNode*> *set, int x, int y) {
  for (int i = 0; i < set->size(); i++) {
    auto node = set->get(i);
    if (node->x == x && node->y == y) {
      return i;
    }
  }

  return -1;
}

const Position* Pathfinder::GetNeighbourPositions(int x, int y) {
  Position* neighbourPositions = new Position[8];

  // Top row
  neighbourPositions[0] = Position(x - 1, y - 1);
  neighbourPositions[1] = Position(x, y - 1);
  neighbourPositions[2] = Position(x + 1, y - 1);
  // Middle row
  neighbourPositions[3] = Position(x - 1, y);
  neighbourPositions[4] = Position(x + 1, y);
  // Bottom row
  neighbourPositions[5] = Position(x - 1, y + 1);
  neighbourPositions[6] = Position(x, y + 1);
  neighbourPositions[7] = Position(x + 1, y + 1);

  return neighbourPositions;
}

