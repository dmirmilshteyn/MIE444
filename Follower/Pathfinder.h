#include "Arduino.h"
#include "MappingConstants.h"

#ifndef PATHFINDER_H
#define PATHFINDER_H

struct Node {
  Node *parent;
  
  int x;
  int y;
  
  float g;
  // float h; - always 0 for Dijkstra's algorithm, no need to waste memory for this

  bool closed;
};

struct PackedNode {
  float g;
  Node *parent;
};

class Pathfinder { 
  public:
    void Initialize();
  
  private:
    byte *closed_nodes;
  
    float CalculateDistanceBetween(Node nodeA, Node nodeB);
    Node UnpackNode(PackedNode packedNode);
    PackedNode PackNode(Node node);
};

#endif
