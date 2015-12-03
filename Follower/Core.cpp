#include "Core.h"

// Normalizes any number to an arbitrary range 
// by assuming the range wraps around when going below min or above max 
// From: http://stackoverflow.com/a/2021986
double normalise(const double value, const double start, const double end)
{
	const double width = end - start;   //
	const double offsetValue = value - start;   // value relative to 0

	return (offsetValue - (floor(offsetValue / width) * width)) + start;
	// + start to reset back to start of original range
}