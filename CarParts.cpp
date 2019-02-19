#include "CarParts.h"

Engine::Engine() : Engine(100, 850, 7450, 2000, 5000, 6000)
{
	
}

Engine::Engine(float maxPower, float minPossibleRPM, float maxPossibleRPM, float minOptimalRMP, float maxOptimalRMP, float maxPowerRPM)
{
	Engine::maxPower = maxPower;
	Engine::minPossibleRPM = minPossibleRPM;
	Engine::maxPossibleRPM = maxPossibleRPM;
	Engine::minOptimalRMP = minOptimalRMP;
	Engine::maxOptimalRMP = maxOptimalRMP;
	Engine::maxPowerRPM = maxPowerRPM;

	maxTorque = maxPower / maxOptimalRMP * 9549.23;
}


void Engine::accelerator(bool pedal)
{
	if (pedal)
	{
		RPM += 86;
	}
	else
	{
		RPM -= 34;
	}

	if (RPM < minPossibleRPM)
	{
		RPM += 53;
	}

	if (RPM > maxPossibleRPM)
	{
		RPM -= 153;
	}
}

float Engine::getCurrentTorque()
{
	if (RPM < minOptimalRMP)
	{
		return RPM / minOptimalRMP * maxTorque;
	}
	else if (RPM > maxOptimalRMP)
	{
		return maxTorque - (RPM - maxOptimalRMP) / (maxPossibleRPM - maxOptimalRMP) * 0.5 * maxTorque;
	}
	else
	{
		return maxTorque;
	}
}