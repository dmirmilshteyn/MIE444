// IntersectionPathfinder.h

#ifndef _INTERSECTIONPATHFINDER_h
#define _INTERSECTIONPATHFINDER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Core.h"
#include "MappingConstants.h"
#include "Map.h"

struct IntersectionPathfinderResult {
	int *path;
	int size;

	IntersectionPathfinderResult(int *path, int size) : path(path), size(size) { }
};

struct IntersectionPathNode {
	IntersectionPathNode *parent;

	byte intersectionMarkerId;
	byte g;

	IntersectionPathNode(byte intersectionMarkerId, int g, IntersectionPathNode *parent) : intersectionMarkerId(intersectionMarkerId), g(g), parent(parent) { }
};

class IntersectionPathNodeSet {
public:
	int size();
	IntersectionPathNode* get(int i);
	void add(IntersectionPathNode *node);
	IntersectionPathNode* remove(int i);

	IntersectionPathNodeSet();

private:
	int _size;

	int _setCount;
	IntersectionPathNode **_set;
};

class IntersectionPathfinder {
public:
	IntersectionPathfinderResult FindPath(int startingMarkerId, int goalIntersectionId);

private:
	byte *closed_nodes;

	void Initialize();
	void Dispose();

	IntersectionPathfinderResult PerformSearch(int startingMarkerId, int goalIntersectionId);
	int DetermineNextNode(IntersectionPathNodeSet *openSet);
	IntersectionPathfinderResult ReconstructPath(IntersectionPathNode *endingNode);
	int IndexOfSet(IntersectionPathNodeSet *set, int markerId);

	void MarkBlocked(int markerId, bool value);
	bool IsBlocked(int markerId);
};

#endif

