#include "Arduino.h"
#include "MappingConstants.h"

#ifndef PATHFINDER_H
#define PATHFINDER_H

struct Node {
  Node *parent;
  
  byte x;
  byte y;
  
  int g;
  // int h; - always 0 for Dijkstra's algorithm, no need to waste memory for this
};

class Pathfinder { 
  public:
    void Initialize();
    void Dispose();
  
  private:
    byte *closed_nodes;
  
    float CalculateDistanceBetween(Node nodeA, Node nodeB);
};

#endif
