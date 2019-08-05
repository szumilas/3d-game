#pragma once

#include "GlobalStructures.h"
#include "GlobalVariables.h"

#include "Screen2D.h"

class PacejkaModel
{

	struct PacejkaWheelData
	{
		Point position;

		float slipRatio = 0.0f;
		float slipAngle = 0.0f;

		float angularVelocity = 0.0f;

		float longitudinalForce = 0.0f;
		float lateralForce = 0.0f;

		float steeringWheelAngle = 0.0f;

		vector2D localVelocity;
		bool frontWheel = false;

		bool WD = false;
	};

public:

	PacejkaModel();

	void setCarGeometry(float mass, float frontWheelsXoffset, float frontWheelsYoffset, float backWheelsXoffset, float rd, float acceleration, float vMax);
	std::vector<Force> calculateForces(int drivingDir, bool tryAccelerate, bool trySlow, bool tryBreak, const vector2D& vCarGlobal, const vector2D& vCarLocal, const vector2D& aLocal, float angularVelocity, float steeringWheelAngle, float rz);
	void calculateLongitudinalForces();
	void calculateLateralForces();
	void calculateLocalVelocities(const vector2D& vCarGlobal, float angularVelocity, float rz);
	void calculateSlipRatios();
	void calculateSlipAngles();
	void recalculateWheelAngularVelocity(float v);

private:


public:

	std::vector<PacejkaWheelData> allWheels;

	bool carDrifting = false;

private:

	float acceleration; //[m / s^2]
	float vMax; //[m / s]
	float mass;
	float frontWheelsXoffset;
	float frontWheelsYoffset;
	float backWheelsXoffset;
	float rd;

	//Coefficients

	float B_Stiffness = 10.0f;
	float C_Shape_Longitudinal = 1.65f;
	float C_Shape_Lateral = 1.3f;
	float D_Peak = 1.0f;
	float E_Curvature = 0.97f;

};