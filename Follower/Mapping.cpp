#include "Arduino.h"
#include "Constants.h"
#include "Mapping.h"
#include "Map.h"

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

