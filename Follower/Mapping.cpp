#include "Arduino.h"
#include "Constants.h"
#include "Mapping.h"

// Map tiles is stored as a byte array, but should be manipulated as a BIT array.
// This is done to minimize memory usage as the Arduino Uno only has 2048 bytes available. 
byte * map_tiles;

const int map_tiles_width = (MAP_WIDTH / MAP_RESOLUTION / 8);
const int map_tiles_height = (MAP_HEIGHT / MAP_RESOLUTION / 8);

void InitializeMapping() {
  map_tiles = new byte[map_tiles_width * map_tiles_height]();
}

int CalculateInternalPosition(int x, int y) {
  return (y * map_tiles_width + x) / 8;
}

int CalculateInternalOffset(int x, int y) {
  return (y * map_tiles_width + x) % 8;
}

bool AccessMapElement(int x, int y) {
  int position = CalculateInternalPosition(x, y);
  int offset = CalculateInternalOffset(x, y);

  byte internal_value = map_tiles[position];

  return internal_value & (1 << offset);
}

void SetMapElement(int x, int y, bool value) {
  int position = CalculateInternalPosition(x, y);
  int offset = CalculateInternalOffset(x, y);

  byte internal_value = map_tiles[position];

  if (value) {
    internal_value = internal_value | 1 << offset;
  } else {
    internal_value = internal_value & ~(1 << offset);
  }

  map_tiles[position] = internal_value;
}

