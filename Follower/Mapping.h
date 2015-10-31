#ifndef MAPPING_H
#define MAPPING_H

void InitializeMapping();
int CalculateInternalPosition(int x, int y);
bool AccessMapElement(int x, int y);
void SetMapElement(int x, int y, bool value);

#endif
