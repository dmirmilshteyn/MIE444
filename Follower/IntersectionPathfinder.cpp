
#include "IntersectionPathfinder.h"
#include "MemoryFree.h"
#include "Localization.h"

IntersectionPathfinder::IntersectionPathfinder(double headingAngle) {
  _headingAngle = headingAngle;
}

void IntersectionPathfinder::Initialize(IntersectionPathNodeSet *trackingSet) {
  // TODO: Create a constant that defines the max # of intersection ids

  closed_nodes = new byte[INTERSECTION_MARKER_COUNT] {};

  // Block off the middle area of the map because it makes the robot sad
  closed_nodes[44] = 1;
  closed_nodes[49] = 1;

  // Close off the TLeft because it causes conflicts
  closed_nodes[19] = 1;
  closed_nodes[22] = 1;
}

void IntersectionPathfinder::Dispose(IntersectionPathNodeSet *trackingSet) {
  delete[] closed_nodes;

  for (int i = 0; i < trackingSet->setSize(); i++) {
    if (trackingSet->get(i) != NULL) {
      IntersectionPathNode *currentNode = trackingSet->remove(i);
      delete currentNode;
    }
  }
}

IntersectionPathfinderResult IntersectionPathfinder::FindPath(int startingMarkerId, int goalIntersectionId) {
  IntersectionPathNodeSet *trackingSet = new IntersectionPathNodeSet();

  Initialize(trackingSet);
  auto path = PerformSearch(startingMarkerId, goalIntersectionId, trackingSet);
  Dispose(trackingSet);

  delete trackingSet;

  return path;
}

IntersectionPathfinderResult IntersectionPathfinder::PerformSearch(int startingMarkerId, int goalIntersectionId, IntersectionPathNodeSet *trackingSet) {
  IntersectionPathNodeSet *openSet = new IntersectionPathNodeSet();

  // Add the starting position
  IntersectionPathNode *startingNode = new IntersectionPathNode(startingMarkerId, 0, NULL);

  openSet->add(startingNode);
  trackingSet->add(startingNode);

  int liveNodeCount = 1;

  while (openSet->size() > 0) {
    int currentNodeIndex = DetermineNextNode(openSet);

    IntersectionPathNode *currentNode = openSet->remove(currentNodeIndex);

    if (pgm_read_byte(&(intersections[currentNode->intersectionMarkerId].id)) == goalIntersectionId) {
      delete openSet;

      return ReconstructPath(currentNode);
    }

    MarkBlocked(currentNode->intersectionMarkerId, true);

    for (int y = 0; y < INTERSECTION_MARKER_COUNT; y++) {
      int cost = (int)(int8_t)pgm_read_byte(&(intersection_graph[y][currentNode->intersectionMarkerId][0]));

      // Check: is cost > -1? If so, this node is a neighbour of the current node
      if (cost > -1) {

        // Check if the node has been closed by the pathfinder
        if (IsBlocked(y)) {
          continue;
        }

        int totalCost = currentNode->g + cost;
        int neighbourIndex = IndexOfSet(openSet, y);

        if (neighbourIndex == -1) {
          // Discover a new node
          IntersectionPathNode *setNode = new IntersectionPathNode(y, totalCost, currentNode);
          liveNodeCount++;

          openSet->add(setNode);
          trackingSet->add(setNode);
        }
        else {
          IntersectionPathNode *neighbourNode = openSet->get(neighbourIndex);

          if (totalCost <= neighbourNode->g) {
            neighbourNode->parent = currentNode;
            neighbourNode->g = totalCost;
          }
        }
      }
    }
  }

  delete openSet;

  return IntersectionPathfinderResult(NULL, NULL, 0, -1);
}

int IntersectionPathfinder::IndexOfSet(IntersectionPathNodeSet *set, int markerId) {
  for (int i = 0; i < set->setSize(); i++) {
    IntersectionPathNode *node = set->get(i);
    if (node != NULL) {
      if (node->intersectionMarkerId == markerId) {
        return i;
      }
    }
  }

  return -1;
}

int IntersectionPathfinder::DetermineNextNode(IntersectionPathNodeSet *openSet) {
  IntersectionPathNode *bestNode = NULL;
  int setIndex = -1;

  for (int i = 0; i < openSet->setSize(); i++) {
    IntersectionPathNode *testNode = openSet->get(i);
    if (testNode != NULL) {
      if (setIndex == -1 || testNode->g < bestNode->g) {
        bestNode = testNode;
        setIndex = i;
      }
    }
  }

  return setIndex;
}

IntersectionPathfinderResult IntersectionPathfinder::ReconstructPath(IntersectionPathNode *endingNode) {
	int depth = 0;
	IntersectionPathNode *currentNode = endingNode;
	while (currentNode != NULL) {
		depth++;
		currentNode = currentNode->parent;
	}

	currentNode = endingNode;

	// TODO: Properly dispose of these paths
	int *path = new int[depth];
	for (int i = depth - 1; i >= 0; i--) {
		path[i] = currentNode->intersectionMarkerId;

		currentNode = currentNode->parent;
	}

	currentNode = endingNode;
	double testHeadingAngle = _headingAngle;

	float *pathTurns = new float[depth];

	//  { 14, -1, M_PI / 2 }, // Starting with TRight
	//  { 21, -1, -M_PI / 2 }, // Starting with TLeft
	//  { 39, 38,  M_PI / 4 }, // Starting with T, then Left
	//  { 39, 38,  M_PI / 4 }, // Starting with T, then TLeft (this is an error case)
	//  { 25, 24, 0 } // Starting with T, then TRight

  

	byte upcomingIntersectionX = pgm_read_byte(&(intersections[path[1]].intersectionX));
	byte upcomingIntersectionY = pgm_read_byte(&(intersections[path[1]].intersectionY));

	byte currentIntersectionX = pgm_read_byte(&(intersections[path[0]].intersectionX));
	byte currentIntersectionY = pgm_read_byte(&(intersections[path[0]].intersectionY));

	double xDiff = upcomingIntersectionX - currentIntersectionX;
	double yDiff = upcomingIntersectionY - currentIntersectionY;

	double upcomingAngle = atan2(yDiff, xDiff);

	double previousAngle = _headingAngle;//pathLocation[currentPath][2];

	Serial.print(" pre ");
	Serial.print(previousAngle);
	Serial.print(" up ");
	Serial.println(upcomingAngle);
	double angle = normalise(upcomingAngle, -M_PI, M_PI) - normalise(previousAngle, -M_PI, M_PI);

	if (abs(angle) < 20 * M_PI / 180) {
		pathTurns[0] = PATH_STRAIGHT;
	}
	else if (angle > 0) {
		pathTurns[0] = PATH_RIGHT;
	}
	else if (angle < 0) {
		pathTurns[0] = PATH_LEFT;
	}
	double previousIntersectionX = 0;
	double previousIntersectionY = 0;
	for (int i = 2; i < depth; i++) {
		upcomingIntersectionX = pgm_read_byte(&(intersections[path[i]].intersectionX));
		upcomingIntersectionY = pgm_read_byte(&(intersections[path[i]].intersectionY));

		currentIntersectionX = pgm_read_byte(&(intersections[path[i - 1]].intersectionX));
		currentIntersectionY = pgm_read_byte(&(intersections[path[i - 1]].intersectionY));

		previousIntersectionX = pgm_read_byte(&(intersections[path[i - 2]].intersectionX));
		previousIntersectionY = pgm_read_byte(&(intersections[path[i - 2]].intersectionY));

		xDiff = upcomingIntersectionX - currentIntersectionX;
		yDiff = upcomingIntersectionY - currentIntersectionY;

		upcomingAngle = atan2(yDiff, xDiff);

		xDiff = currentIntersectionX - previousIntersectionX;
		yDiff = currentIntersectionY - previousIntersectionY;

		previousAngle = atan2(yDiff, xDiff);

		Serial.print(" pre ");
		Serial.print(previousAngle);
		Serial.print(" up ");
		Serial.println(upcomingAngle);

		angle = normalise(upcomingAngle, -M_PI, M_PI) - normalise(previousAngle, -M_PI, M_PI);


		if (abs(angle) < 20 * M_PI / 180) {
			pathTurns[i - 1] = PATH_STRAIGHT;
		}
		else if (angle > 0) {
			pathTurns[i - 1] = PATH_RIGHT;
		}
		else if (angle < 0) {
			pathTurns[i - 1] = PATH_LEFT;
		}
		testHeadingAngle = angle;

		_headingAngle;
		//pathTurns[i - 1] = angle;
	}
	pathTurns[depth - 1] = 0;

	return IntersectionPathfinderResult(path, pathTurns, depth, endingNode->g);
}



void IntersectionPathfinder::MarkBlocked(int markerId, bool value) {
  /*int position = markerId / 8;
    int offset = markerId % 8;

    byte internal_value = closed_nodes[position];

    if (value) {
  	internal_value = internal_value | 1 << offset;
    }
    else {
  	internal_value = internal_value & ~(1 << offset);
    }

    closed_nodes[position] = internal_value;*/
  closed_nodes[markerId] = 1;
}

bool IntersectionPathfinder::IsBlocked(int markerId) {
  /*int position = markerId / 8;
    int offset = markerId % 8;

    byte internal_value = closed_nodes[position];

    return internal_value & (1 << offset);*/
  return (closed_nodes[markerId] == 1);
}

IntersectionPathNodeSet::IntersectionPathNodeSet() {
  _setCount = 100;
  _size = 0;
  _set = new IntersectionPathNode*[_setCount] {};
}

IntersectionPathNodeSet::~IntersectionPathNodeSet() {
  delete[] _set;
}

int IntersectionPathNodeSet::size() {
  return _size;
}

int IntersectionPathNodeSet::setSize() {
  return _setCount;
}

IntersectionPathNode* IntersectionPathNodeSet::get(int i) {
  return _set[i];
}

void IntersectionPathNodeSet::add(IntersectionPathNode *node) {
  for (int i = 0; i < _setCount; i++) {
    if (_set[i] == NULL) {
      _set[i] = node;

      _size++;
      break;
    }
  }
}

IntersectionPathNode* IntersectionPathNodeSet::remove(int i) {
  IntersectionPathNode *result = _set[i];

  _size--;
  _set[i] = NULL;

  return result;
}
