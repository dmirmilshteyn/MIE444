/**********************************************
 ** This is a generated file. Do not modify. **
 **********************************************/

#ifndef MAP_H
#define MAP_H

// Map tiles are stored as a byte array, but should be manipulated as a BIT array.
// This is done to minimize memory usage as the Arduino Uno only has 2048 bytes available.

const int map_tiles_width = 12;
const int map_tiles_height = 8;

// Array size = (MAP_WIDTH / MAP_RESOLUTION / 8) * (MAP_HEIGHT / MAP_RESOLUTION / 8)
byte map_tiles[96] = {
0, 0, 0, 8, 0, 196, 63, 1, 144, 8, 16, 4, 4, 48, 0, 
32, 130, 16, 65, 0, 1, 9, 32, 7, 128, 0, 136, 8, 132, 4, 
130, 130, 192, 64, 64, 32, 96, 0, 80, 0, 36, 4, 48, 2, 6, 
190, 0, 0, 64, 16, 32, 8, 0, 0, 0, 0, 4, 127, 128, 0, 
2, 0, 1, 0, 23, 0, 8, 0, 2, 128, 2, 0, 1, 128, 1, 
32, 0, 72, 0, 16, 0, 40, 224, 255, 1, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0
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
