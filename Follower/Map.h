/**********************************************
 ** This is a generated file. Do not modify. **
 **********************************************/

#ifndef MAP_H
#define MAP_H

// Map tiles are stored as a byte array, but should be manipulated as a BIT array.
// This is done to minimize memory usage as the Arduino Uno only has 2048 bytes available.

const int map_tiles_width = 25;
const int map_tiles_height = 25;

// Array size = (MAP_WIDTH / MAP_RESOLUTION / 8) * (MAP_HEIGHT / MAP_RESOLUTION / 8)
byte map_tiles[625];

#endif