#pragma once

#include <vector>

class Engine
{

public:
	
	Engine();
	Engine(float maxPower, float minPossibleRPM, float maxPossibleRPM, float minOptimalRMP, float maxOptimalRMP, float maxPowerRPM);


	float getRPM() { return RPM; }
	void accelerator(bool pedal);
	float getCurrentTorque();
	inline float getMaxTorque() { return maxTorque; }

private:


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

	inline float getTransmission(unsigned int gear) { return gears[gear]; }
	inline float getCurrentTransmission() { return gears[currentGear]; }
	inline float getTopTransmission() { return gears.back(); }
	inline float getMainTransmission() { return mainTransmission; }
	inline void gearUp() { if (currentGear < gears.size() - 1) currentGear++; }
	inline void gearDown() { if (currentGear > 0) currentGear--; }

private:


public:


private:

	unsigned int currentGear = 0;
	std::vector<float> gears{0.0f, 3.5f, 2.0f, 1.4f, 1.1f, 0.7f};
	float mainTransmission = 4.0f;

};