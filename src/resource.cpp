#include "resource.h"

Timer* timer = NULL;

float RandomRange(float min,float max) {
	// Scale the range to [0,1]
	float baserange = (float)rand()/(float)RAND_MAX;
	
	// Then scale to [0,(max-min)]
	float scalerange = baserange*(max - min);
	
	// And move by min to [min,max]
	float finalrange = scalerange + min;
	return finalrange;
}
