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
	float *pathTurns;
	int size;

	IntersectionPathfinderResult(int *path, float *pathTurns, int size) : path(path), pathTurns(pathTurns), size(size) { }
};

struct IntersectionPathNode {
	IntersectionPathNode *parent;

	int intersectionMarkerId;
	int g;

	IntersectionPathNode(int intersectionMarkerId, int g, IntersectionPathNode *parent) : intersectionMarkerId(intersectionMarkerId), g(g), parent(parent) { }
};

class IntersectionPathNodeSet {
public:
	int size();
	int setSize();
	IntersectionPathNode* get(int i);
	void add(IntersectionPathNode *node);
	IntersectionPathNode* remove(int i);

	IntersectionPathNodeSet();
  ~IntersectionPathNodeSet();

private:
	int _size;

	int _setCount;
	IntersectionPathNode **_set;
};

class IntersectionPathfinder {
public:
	IntersectionPathfinderResult FindPath(int startingMarkerId, int goalIntersectionId);

	IntersectionPathfinder(double headingAngle);
private:
	byte *closed_nodes;
	double _headingAngle;

	void Initialize(IntersectionPathNodeSet *trackingSet);
	void Dispose(IntersectionPathNodeSet *tracking);

	IntersectionPathfinderResult PerformSearch(int startingMarkerId, int goalIntersectionId, IntersectionPathNodeSet *trackingSet);
	int DetermineNextNode(IntersectionPathNodeSet *openSet);
	IntersectionPathfinderResult ReconstructPath(IntersectionPathNode *endingNode);
	int IndexOfSet(IntersectionPathNodeSet *set, int markerId);

	void MarkBlocked(int markerId, bool value);
	bool IsBlocked(int markerId);
};

#endif

