/**********************************************
 ** This is a generated file. Do not modify. **
 **********************************************/

#ifndef MAP_H
#define MAP_H

// Map tiles are stored as a byte array, but should be manipulated as a BIT array.
// This is done to minimize memory usage as the Arduino Uno only has 2048 bytes available.

const int map_tiles_width = 20;
const int map_tiles_height = 15;

// Array size = (MAP_WIDTH / MAP_RESOLUTION / 8) * (MAP_HEIGHT / MAP_RESOLUTION / 8)
byte map_tiles[300] = {
0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 240, 15, 0, 
0, 60, 0, 0, 7, 0, 0, 0, 128, 7, 192, 63, 0, 0, 0, 
0, 60, 0, 0, 7, 0, 0, 0, 0, 252, 0, 0, 0, 0, 0, 
0, 60, 0, 0, 7, 0, 63, 0, 0, 0, 254, 1, 0, 0, 0, 
0, 248, 0, 0, 7, 0, 128, 15, 0, 0, 0, 252, 0, 0, 0, 
0, 128, 7, 0, 0, 240, 15, 0, 0, 0, 224, 3, 0, 0, 0, 
0, 240, 121, 0, 0, 0, 0, 254, 1, 128, 15, 0, 0, 0, 0, 
0, 62, 0, 0, 0, 0, 0, 0, 192, 63, 0, 0, 0, 0, 0, 
0, 15, 0, 0, 0, 0, 0, 0, 240, 1, 248, 3, 0, 0, 0, 
0, 15, 0, 0, 0, 0, 0, 192, 7, 0, 255, 255, 1, 0, 0, 
0, 15, 0, 0, 0, 0, 0, 126, 0, 0, 0, 0, 0, 0, 0, 
0, 15, 0, 0, 0, 0, 0, 30, 0, 0, 0, 0, 0, 0, 0, 
0, 30, 0, 0, 0, 0, 0, 30, 0, 0, 0, 0, 0, 0, 0, 
0, 224, 3, 0, 0, 0, 0, 30, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 192, 255, 255, 255, 1, 30, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 224, 1, 30, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 224, 1, 254, 1, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 224, 1, 30, 30, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 224, 1, 30, 224, 3, 0, 0, 0, 0, 0
};

struct intersection {
  short TopLeft;
  short TopRight;
  short BottomLeft;
  short BottomRight;
};

intersection intersections[0] = {
};

#endif
