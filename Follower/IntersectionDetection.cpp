#include "IntersectionDetection.h"

void ReadIntersectionSensors() {
	int sensorA = analogRead(LINE_SENSOR_1);
	int sensorB = analogRead(LINE_SENSOR_2);
	int sensorC = analogRead(LINE_SENSOR_3);

	publishIntersectionDetectionData(sensorA, sensorB, sensorC);
}