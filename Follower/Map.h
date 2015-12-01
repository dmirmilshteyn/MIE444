/**********************************************
 ** This is a generated file. Do not modify. **
 **********************************************/

#ifndef MAP_H
#define MAP_H

#include "Arduino.h"
#include "MappingConstants.h"

// Map tiles are stored as a byte array, but should be manipulated as a BIT array.
// This is done to minimize memory usage as the Arduino Uno only has 2048 bytes available.

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

const PROGMEM intersection_marker intersections[INTERSECTION_MARKER_COUNT] = {
  { 0, 5, 16, 8, 17, 6, 6, 15 }, 
  { 0, 7, 14, 8, 16, 6, 6, 15 }, 
  { 0, 4, 14, 8, 14, 6, 6, 15 }, 
  { 0, 4, 14, 4, 17, 6, 6, 15 }, 
  { 1, 4, 7, 8, 7, 1, 6, 5 }, 
  { 1, 7, 2, 8, 7, 2, 6, 5 }, 
  { 2, 27, 5, 24, 9, 2, 28, 6 }, 
  { 2, 28, 4, 33, 8, 1, 28, 6 }, 
  { 3, 45, 2, 45, 6, 3, 46, 5 }, 
  { 3, 44, 7, 49, 7, 5, 46, 5 }, 
  { 3, 49, 1, 49, 7, 4, 46, 5 }, 
  { 4, 65, 20, 70, 20, 2, 68, 21 }, 
  { 4, 70, 20, 70, 24, 1, 68, 21 }, 
  { 5, 85, 20, 85, 24, 5, 86, 22 }, 
  { 5, 84, 19, 90, 20, 3, 86, 22 }, 
  { 5, 85, 25, 89, 25, 4, 86, 22 }, 
  { 6, 85, 27, 89, 27, 3, 86, 28 }, 
  { 6, 85, 30, 89, 30, 4, 86, 28 }, 
  { 6, 84, 26, 84, 30, 5, 86, 28 }, 
  { 7, 85, 33, 85, 37, 2, 86, 35 }, 
  { 7, 85, 33, 89, 33, 3, 86, 35 }, 
  { 7, 85, 37, 89, 37, 4, 86, 35 }, 
  { 8, 71, 30, 78, 30, 5, 74, 28 }, 
  { 8, 78, 26, 79, 30, 4, 74, 28 }, 
  { 8, 69, 26, 69, 30, 3, 74, 28 }, 
  { 9, 57, 30, 61, 30, 5, 58, 28 }, 
  { 9, 56, 26, 56, 30, 3, 58, 28 }, 
  { 9, 61, 26, 62, 30, 4, 58, 28 }, 
  { 10, 49, 26, 50, 30, 4, 47, 30 }, 
  { 10, 45, 32, 49, 33, 4, 47, 30 }, 
  { 10, 45, 27, 45, 32, 3, 47, 30 }, 
  { 11, 38, 37, 36, 33, 6, 35, 35 }, 
  { 11, 35, 39, 32, 35, 6, 35, 35 }, 
  { 11, 35, 39, 39, 37, 6, 35, 35 }, 
  { 11, 32, 35, 36, 31, 6, 35, 35 }, 
  { 12, 40, 46, 42, 40, 2, 42, 44 }, 
  { 12, 42, 42, 46, 48, 1, 42, 44 }, 
  { 13, 20, 42, 22, 47, 1, 19, 43 }, 
  { 13, 16, 45, 20, 41, 2, 19, 43 }, 
  { 14, 9, 38, 13, 40, 5, 11, 37 }, 
  { 14, 13, 40, 15, 36, 4, 11, 37 }, 
  { 14, 8, 38, 11, 33, 3, 11, 37 }, 
  { 15, 3, 32, 9, 32, 2, 6, 33 }, 
  { 15, 7, 32, 7, 37, 1, 6, 33 }, 
  { 16, 12, 13, 13, 17, 3, 14, 15 }, 
  { 16, 12, 17, 16, 17, 5, 14, 15 }, 
  { 16, 16, 13, 16, 17, 4, 14, 15 }, 
  { 17, 20, 13, 20, 17, 1, 21, 16 }, 
  { 17, 21, 17, 26, 18, 2, 21, 16 }, 
  { 18, 26, 19, 26, 26, 4, 24, 22 }, 
  { 18, 21, 24, 26, 25, 3, 24, 22 }, 
  { 18, 21, 20, 26, 19, 4, 24, 22 }, 
  { 19, 20, 24, 19, 28, 2, 21, 25 }, 
  { 19, 21, 25, 26, 26, 1, 21, 25 }
};

extern const int8_t intersection_graph[INTERSECTION_MARKER_COUNT][INTERSECTION_MARKER_COUNT][2];

#endif
