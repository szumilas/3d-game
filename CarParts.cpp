#include "CarParts.h"

Engine::Engine(CarBrand carBrand) : Engine(carDB.at(carBrand).power, 850, 7450, 2000, 5000, 6000)
{

}

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

	if (maxPower > 300)
	{
		Engine::maxPossibleRPM = 8000;
		Engine::maxOptimalRMP = 6500;
	}
}

float Engine::getCurrentTorque()
{
	return getTorque(RPM);
}

float Engine::getTorque(float RPM)
{
	if (RPM < minOptimalRMP)
	{
		return RPM / minOptimalRMP * maxTorque;
	}
	else if (RPM > maxOptimalRMP)
	{
		return maxTorque - (RPM - maxOptimalRMP) / (maxPossibleRPM - maxOptimalRMP) * 1.0 * maxTorque;
	}
	else
	{
		return maxTorque;
	}
}

void Engine::setRPM(float newRPM)
{
	if (newRPM < minPossibleRPM)
		RPM = minPossibleRPM;
	else if (newRPM > maxPossibleRPM)
		RPM = maxPossibleRPM;
	else
		RPM = newRPM;
}

bool Engine::nextGearDrivingForceBigger(float mainTransmission, float currentTransmission, float nextTransmission, float rd, float v)
{
	float currentRMP = calculateRMP(mainTransmission * currentTransmission, rd, v);
	float nextRMP = calculateRMP(mainTransmission * nextTransmission, rd, v);

	float currentTorgue = getTorque(currentRMP);
	float nextTorgue = getTorque(nextRMP);

	if (nextTransmission * nextTorgue > currentTransmission * currentTorgue)
		return true;
	else
		return false;
}

bool Engine::previousGearDrivingForceBigger(float mainTransmission, float currentTransmission, float previousTransmission, float rd, float v)
{
	float currentRMP = calculateRMP(mainTransmission * currentTransmission, rd, v);
	float previousRMP = calculateRMP(mainTransmission * previousTransmission, rd, v);

	float currentTorgue = getTorque(currentRMP);
	float previousTorgue = getTorque(previousRMP);

	if (previousTransmission * previousTorgue > currentTransmission * currentTorgue)
		return true;
	else
		return false;
}

GearBox::GearBox(CarBrand carBrand)
{
	gears = carDB.at(carBrand).gears;
	mainTransmission = carDB.at(carBrand).mainTransmission;
}