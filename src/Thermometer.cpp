// include this library's description file
#include "Thermometer.h"
#include <Arduino.h>

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

Thermometer::Thermometer()
{
}

float Thermometer::readProbe(int probeId)
{
	return adc.read(probeId);
}
