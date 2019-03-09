#pragma once

#include <vector>

#include "GlobalStructures.h"
#include "carDB.h"

class Engine
{

	friend class CarGauge;

public:
	
	Engine();
	Engine(CarBrand carBrand);
	Engine(float maxPower, float minPossibleRPM, float maxPossibleRPM, float minOptimalRMP, float maxOptimalRMP, float maxPowerRPM);


	float getRPM() { return RPM; }
	void setRPM(float newRPM);
	float getCurrentTorque();
	inline float getMaxTorque() { return maxTorque; }
	inline void changeRPM(float currentTransmission, float nextTransmission) { RPM *= (nextTransmission / currentTransmission); }
	bool nextGearDrivingForceBigger(float mainTransmission, float currentTransmission, float nextTransmission, float rd, float v);
	bool previousGearDrivingForceBigger(float mainTransmission, float currentTransmission, float previousTransmission, float rd, float v);
	static float calculateRMP(float totalTransmission, float rd, float v) { return totalTransmission * 30 * v / PI / rd; }

private:

	float getTorque(float RPM);

public:


private:
	float RPM = 1000;

	float maxPower; //[kW]
	float minPossibleRPM; //[1 / min]
	float maxPossibleRPM; //[1 / min]
	float minOptimalRMP; //[1 / min]
	float maxOptimalRMP; //[1 / min]
	float maxPowerRPM; //[1 / min]

	float maxTorque; //[kN * m]
};

class GearBox
{
public:

	GearBox(CarBrand carBrand);

	inline float getTransmission(unsigned int gear) { return gears[gear]; }
	inline float getCurrentTransmission() { return gears[currentGear]; }
	inline float getNextTransmission() { if (currentGear == gears.size() - 1) return gears[currentGear]; return gears[currentGear + 1]; }
	inline float getPreviousTransmission() { if (currentGear == 0) return gears[currentGear]; return gears[currentGear - 1]; }
	inline float getTopTransmission() { return gears.back(); }
	inline float getMainTransmission() { return mainTransmission; }
	inline void gearUp() { if (currentGear < gears.size() - 1) currentGear++; }
	inline void gearDown() { if (currentGear > 0) currentGear--; }
	inline unsigned int getCurrentGear() { return currentGear; };

private:


public:


private:

	unsigned int currentGear = 0;
	std::vector<float> gears{0.0f, 3.5f, 2.0f, 1.4f, 1.1f, 0.7f};
	float mainTransmission = 4.0f;

};