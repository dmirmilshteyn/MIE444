#include "Pathfinder.h"

void Pathfinder::Initialize() {
  closed_nodes = new byte[map_tiles_width * map_tiles_height] { 0 };
}

float Pathfinder::CalculateDistanceBetween(Node nodeA, Node nodeB) {
  return 0;
}

Node Pathfinder::UnpackNode(PackedNode packedNode) {
  return Node();
}
 
PackedNode Pathfinder::PackNode(Node node) {
  return PackedNode();
}

