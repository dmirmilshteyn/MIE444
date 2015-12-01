#include "IntersectionPathfinder.h"

void IntersectionPathfinder::Initialize() {
	// TODO: Create a constant that defines the max # of intersection ids

	closed_nodes = new byte[(int)ceil(INTERSECTION_MARKER_COUNT / 8.0)];
}

void IntersectionPathfinder::Dispose() {
	delete[] closed_nodes;
}

IntersectionPathfinderResult IntersectionPathfinder::FindPath(int startingMarkerId, int goalIntersectionId) {
	Initialize();
	auto path = PerformSearch(startingMarkerId, goalIntersectionId);
	Dispose();

	return path;
}

IntersectionPathfinderResult IntersectionPathfinder::PerformSearch(int startingMarkerId, int goalIntersectionId) {
	IntersectionPathNodeSet *openSet = new IntersectionPathNodeSet();

	// Add the starting position
	openSet->add(new IntersectionPathNode(startingMarkerId, 0, NULL));

	while (openSet->size() > 0) {
		int currentNodeIndex = DetermineNextNode(openSet);

		IntersectionPathNode *currentNode = openSet->remove(currentNodeIndex);

		if (pgm_read_byte(&(intersections[currentNode->intersectionMarkerId].id)) == goalIntersectionId) {
			return ReconstructPath(currentNode);
		}

		MarkBlocked(currentNode->intersectionMarkerId, true);

		for (int y = 0; y < INTERSECTION_MARKER_COUNT; y++) {
			int cost = pgm_read_byte(&(intersection_graph[y][currentNode->intersectionMarkerId][0]));

			// Check: is cost > -1? If so, this node is a neighbour of the current node
			if (cost > -1) {

				// Check if the node has been closed by the pathfinder
				if (IsBlocked(y)) {
					continue;
				}

				int totalCost = /* TODO: previous cost */0 + cost;
				int neighbourIndex = IndexOfSet(openSet, y);
				if (neighbourIndex == -1) {
					// Discover a new node
					IntersectionPathNode *setNode = new IntersectionPathNode(y, totalCost, currentNode);

					openSet->add(setNode);
				}
				else {
					auto neighbourNode = openSet->get(neighbourIndex);

					if (totalCost <= neighbourNode->g) {
						neighbourNode->parent = currentNode;
						neighbourNode->g = totalCost;
					}
				}
			}
		}
	}

	return IntersectionPathfinderResult(NULL, 0);
}

int IntersectionPathfinder::IndexOfSet(IntersectionPathNodeSet *set, int markerId) {
	for (int i = 0; i < set->size(); i++) {
		auto node = set->get(i);
		if (node->intersectionMarkerId == markerId) {
			return i;
		}
	}

	return -1;
}

int IntersectionPathfinder::DetermineNextNode(IntersectionPathNodeSet *openSet) {
	IntersectionPathNode *bestNode = NULL;
	int setIndex = -1;

	for (int i = 0; i < openSet->size(); i++) {
		IntersectionPathNode *testNode = openSet->get(i);
		if (setIndex == -1 || testNode->g < bestNode->g) {
			bestNode = testNode;
			setIndex = i;
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

	int *path = new int[depth]();
	for (int i = depth - 1; i >= 0; i--) {
		path[i] = currentNode->intersectionMarkerId;

		currentNode = currentNode->parent;
	}
	return IntersectionPathfinderResult(path, depth);
}

void IntersectionPathfinder::MarkBlocked(int markerId, bool value) {
	int position = markerId / 8;
	int offset = markerId % 8;

	byte internal_value = closed_nodes[position];

	if (value) {
		internal_value = internal_value | 1 << offset;
	}
	else {
		internal_value = internal_value & ~(1 << offset);
	}

	closed_nodes[position] = internal_value;
}

bool IntersectionPathfinder::IsBlocked(int markerId) {
	int position = markerId / 8;
	int offset = markerId % 8;

	byte internal_value = closed_nodes[position];

	return internal_value & (1 << offset);
}

IntersectionPathNodeSet::IntersectionPathNodeSet() {
	_setCount = 100;
	_set = new IntersectionPathNode*[_setCount] {};
}

int IntersectionPathNodeSet::size() {
	return _size;
}

IntersectionPathNode* IntersectionPathNodeSet::get(int i) {
	return _set[i];
}

void IntersectionPathNodeSet::add(IntersectionPathNode *node) {
	for (int i = 0; i < _setCount; i++) {
		if (_set[i] == NULL) {
			_set[i] = node;
		}
	}
}

IntersectionPathNode* IntersectionPathNodeSet::remove(int i) {
	auto result = _set[i];

	_set[i] = NULL;

	return result;
}
