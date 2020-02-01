#include "Mcp3208.h"

class Thermometer 
{
    Mcp3208 adc;
public:
    Thermometer();
    float readProbe(int probeId);
};