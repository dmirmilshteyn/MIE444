/**********************************************
 ** This is a generated file. Do not modify. **
 **********************************************/

#ifndef MAP_H
#define MAP_H

#include "Arduino.h"
#include "MappingConstants.h"
// Map tiles are stored as a byte array, but should be manipulated as a BIT array.
// This is done to minimize memory usage as the Arduino Uno only has 2048 bytes available.

// Array size = ((MAP_WIDTH / MAP_RESOLUTION) * (MAP_HEIGHT / MAP_RESOLUTION) / 8)
const PROGMEM byte map_tiles[675] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 8, 0, 240, 255, 255, 15, 0, 0, 0, 0, 255, 255, 1, 
252, 255, 255, 255, 0, 0, 0, 6, 252, 255, 63, 252, 0, 24, 128, 
15, 0, 0, 24, 48, 0, 224, 123, 0, 96, 0, 112, 0, 0, 96, 
192, 0, 0, 126, 0, 128, 1, 128, 3, 0, 128, 1, 3, 0, 224, 
3, 0, 6, 0, 28, 0, 0, 6, 12, 0, 0, 12, 0, 24, 0, 
96, 0, 0, 24, 48, 0, 0, 0, 0, 96, 0, 0, 3, 0, 96, 
192, 0, 0, 0, 0, 128, 1, 0, 12, 0, 128, 1, 3, 0, 0, 
0, 0, 6, 0, 112, 0, 0, 6, 28, 0, 0, 0, 0, 24, 0, 
128, 1, 0, 24, 48, 0, 0, 0, 0, 96, 0, 0, 6, 0, 96, 
255, 255, 255, 3, 0, 128, 1, 0, 24, 0, 128, 1, 3, 3, 14, 
0, 0, 6, 0, 96, 0, 0, 6, 12, 12, 56, 0, 0, 24, 0, 
128, 1, 0, 24, 48, 48, 224, 0, 0, 96, 0, 0, 6, 0, 96, 
192, 192, 128, 3, 0, 128, 1, 0, 24, 0, 128, 1, 3, 3, 30, 
0, 0, 6, 0, 96, 0, 0, 6, 12, 12, 248, 0, 0, 24, 0, 
128, 255, 255, 31, 48, 112, 224, 3, 0, 96, 0, 0, 254, 255, 127, 
192, 128, 135, 31, 0, 0, 0, 0, 0, 0, 128, 1, 3, 56, 238, 
0, 0, 0, 0, 0, 0, 0, 6, 12, 192, 59, 3, 0, 0, 0, 
0, 0, 0, 24, 48, 0, 254, 28, 0, 0, 0, 0, 0, 0, 96, 
192, 0, 224, 227, 0, 0, 255, 255, 255, 255, 255, 1, 3, 0, 15, 
3, 0, 254, 255, 255, 255, 255, 7, 12, 0, 48, 28, 0, 62, 128, 
1, 224, 1, 24, 48, 0, 128, 224, 0, 252, 0, 6, 0, 62, 96, 
192, 0, 0, 0, 7, 188, 1, 24, 0, 224, 131, 1, 3, 0, 0, 
24, 56, 6, 96, 0, 0, 62, 6, 12, 0, 0, 224, 120, 24, 128, 
1, 0, 224, 27, 240, 0, 0, 0, 119, 48, 0, 6, 0, 0, 126, 
192, 31, 0, 0, 248, 192, 0, 24, 0, 0, 224, 1, 240, 1, 0, 
240, 0, 3, 96, 0, 0, 0, 6, 0, 15, 0, 224, 7, 14, 128, 
1, 0, 0, 24, 0, 126, 0, 224, 25, 24, 0, 6, 0, 0, 96, 
0, 156, 3, 192, 227, 96, 0, 24, 0, 0, 128, 1, 48, 28, 192, 
3, 135, 1, 96, 0, 0, 0, 6, 224, 96, 128, 3, 56, 7, 128, 
1, 0, 0, 24, 128, 1, 131, 7, 192, 12, 0, 6, 0, 0, 96, 
0, 3, 140, 7, 0, 55, 0, 24, 0, 0, 128, 1, 14, 112, 15, 
0, 248, 0, 96, 0, 0, 0, 6, 24, 128, 15, 0, 192, 3, 128, 
1, 0, 0, 24, 112, 0, 30, 0, 0, 7, 0, 6, 0, 0, 96, 
224, 0, 24, 0, 0, 24, 0, 24, 0, 0, 128, 129, 1, 0, 0, 
0, 64, 0, 96, 0, 0, 0, 6, 7, 0, 0, 0, 0, 0, 128, 
1, 0, 0, 24, 14, 0, 0, 0, 0, 0, 0, 6, 0, 0, 96, 
24, 0, 0, 0, 0, 0, 0, 24, 0, 0, 128, 113, 0, 0, 0, 
0, 0, 0, 96, 0, 0, 0, 198, 0, 0, 0, 0, 0, 0, 128, 
1, 0, 0, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

#define INTERSECTION_TYPE_NONE 0
#define INTERSECTION_TYPE_RIGHTTURN 1
#define INTERSECTION_TYPE_LEFTTURN 2
#define INTERSECTION_TYPE_TRIGHT 3
#define INTERSECTION_TYPE_TLEFT 4
#define INTERSECTION_TYPE_T 5
#define INTERSECTION_TYPE_CROSS 6

struct intersection_marker {
  byte id;

  byte x1;
  byte y1;
  byte x2;
  byte y2;

  byte type;

  byte intersectionX;
  byte intersectionY;
};

extern const intersection_marker intersections[INTERSECTION_MARKER_COUNT];

struct start_position {
  byte x;
  byte y;
  byte nearestIntersectionId;
};

extern const start_position start_positions[STARTING_POSITION_COUNT];

extern const int8_t intersection_graph[INTERSECTION_MARKER_COUNT][INTERSECTION_MARKER_COUNT][2];

#endif
