#include "PacejkaModel.h"

#include <algorithm>

PacejkaModel::PacejkaModel()
{
	allWheels.resize(4);
}

void PacejkaModel::setCarGeometry(float mass, float frontWheelsXoffset, float frontWheelsYoffset, float backWheelsXoffset, float rd, float acceleration, float vMax)
{
	PacejkaModel::mass = mass;
	PacejkaModel::frontWheelsXoffset = frontWheelsXoffset;
	PacejkaModel::frontWheelsYoffset = frontWheelsYoffset;
	PacejkaModel::backWheelsXoffset = backWheelsXoffset;
	PacejkaModel::rd = rd;
	PacejkaModel::acceleration = acceleration;
	PacejkaModel::vMax = vMax;

	allWheels[frontLeftWheel].position = { frontWheelsXoffset, frontWheelsYoffset, 0.52f };
	allWheels[frontRightWheel].position = { frontWheelsXoffset, -frontWheelsYoffset, 0.52f };
	allWheels[rearLeftWheel].position = { backWheelsXoffset, frontWheelsYoffset, 0.52f };
	allWheels[rearRightWheel].position = { backWheelsXoffset, -frontWheelsYoffset, 0.52f };

	allWheels[frontLeftWheel].WD = true;
	allWheels[frontRightWheel].WD = true;

	allWheels[frontLeftWheel].frontWheel = true;
	allWheels[frontRightWheel].frontWheel = true;
}

std::vector<Force> PacejkaModel::calculateForces(int drivingDir, bool tryAccelerate, bool trySlow, bool tryBreak, const vector2D& vCarGlobal, const vector2D& vCarLocal, const vector2D& aLocal, float angularVelocity, float steeringWheelAngle, float rz)
{
	std::vector<Force> resultForces;

	allWheels[frontLeftWheel].steeringWheelAngle = steeringWheelAngle;
	allWheels[frontRightWheel].steeringWheelAngle = steeringWheelAngle;

	if (tryAccelerate)
	{
		for (auto& wheel : allWheels)
		{
			if (wheel.WD)
			{
				wheel.angularVelocity += (wheel.angularVelocity < 0 ? 2 : 1) * acceleration / rd / FPS;
				if (wheel.angularVelocity > vMax / rd)
					wheel.angularVelocity = vMax / rd;
				if (wheel.angularVelocity < vCarGlobal.x / rd)
					wheel.angularVelocity = vCarGlobal.x / rd;
			}
		}
	}
	else if (trySlow)
	{
		for (auto& wheel : allWheels)
		{
			if (wheel.WD)
			{
				wheel.angularVelocity -= (wheel.angularVelocity > 0 ? 2 : 1) * acceleration / rd / FPS;
				if (wheel.angularVelocity < -vMax / 4 / rd)
					wheel.angularVelocity = -vMax / 4 / rd;
			}
		}
	}
	else if (tryBreak)
	{
		for (auto& wheel : allWheels)
		{
			auto diff = std::max(0.1 * abs(wheel.angularVelocity), 1.0);
			if(wheel.angularVelocity > 0)
				wheel.angularVelocity = std::max(wheel.angularVelocity - diff, 0.0);
			else
				wheel.angularVelocity = std::min(wheel.angularVelocity + diff, 0.0);
		}
	}
	else
	{
		for (auto& wheel : allWheels)
		{
			if (abs(wheel.angularVelocity) < 0.1f)
			{
				wheel.angularVelocity = 0.0f;
			}
			else if (wheel.angularVelocity > 0)
			{
				wheel.angularVelocity -= 2.0 / FPS;
				if (abs(wheel.angularVelocity < 1.0))
					wheel.angularVelocity = 0.0;
			}
			else
			{
				wheel.angularVelocity += 2.0 / FPS;
				if (abs(wheel.angularVelocity > -1.0))
					wheel.angularVelocity = 0.0;
			}
		}
	}

	recalculateWheelAngularVelocity(vCarGlobal.length() * drivingDir);

	calculateLocalVelocities(vCarGlobal, angularVelocity, rz);

	calculateSlipRatios();
	calculateLongitudinalForces();
	
	calculateSlipAngles();
	calculateLateralForces();


	if (humanCar)
	{
		//Screen2D::Instance()->addTestValueToPrint(ColorName::BLACK, -100, 70, "wheel velocity: " + std::to_string(allWheels[frontLeftWheel].angularVelocity), &(Screen2D::Instance()->roboto_modo_regular));
		//Screen2D::Instance()->addTestValueToPrint(ColorName::BLACK, -100, 65, "car velocity x: " + std::to_string(vCarGlobal.x) + "y: " + std::to_string(vCarGlobal.y), &(Screen2D::Instance()->roboto_modo_regular));
		//Screen2D::Instance()->addTestValueToPrint(ColorName::BLACK, -100, 60, "car w: " + std::to_string(angularVelocity), &(Screen2D::Instance()->roboto_modo_regular));
		//Screen2D::Instance()->addTestValueToPrint(ColorName::YELLOW, -100, 55, "[ANGULAR VELOCITY]", &(Screen2D::Instance()->roboto_modo_regular));
		//Screen2D::Instance()->addTestValueToPrint(ColorName::BLACK, -100, 50, "front-left: " + std::to_string(allWheels[frontLeftWheel].lateralForce), &(Screen2D::Instance()->roboto_modo_regular));
		//Screen2D::Instance()->addTestValueToPrint(ColorName::BLACK, -100, 45, "front-right: " + std::to_string(allWheels[frontRightWheel].lateralForce), &(Screen2D::Instance()->roboto_modo_regular));
		//Screen2D::Instance()->addTestValueToPrint(ColorName::BLACK, -100, 40, "rear-left: " + std::to_string(allWheels[rearLeftWheel].lateralForce), &(Screen2D::Instance()->roboto_modo_regular));
		//Screen2D::Instance()->addTestValueToPrint(ColorName::BLACK, -100, 35, "rear-right: " + std::to_string(allWheels[rearRightWheel].lateralForce), &(Screen2D::Instance()->roboto_modo_regular));
	}

	for (auto& wheel : allWheels)
	{
		vector2D force;

		float longitudinalForceCoefficient = 1.0;

		if (wheel.frontWheel && aLocal.x < 0 || !wheel.frontWheel && aLocal.x > 0)
		{
			longitudinalForceCoefficient = 1 + abs(aLocal.x) / 4;
		}

		force.x = wheel.longitudinalForce * cos(wheel.steeringWheelAngle) * longitudinalForceCoefficient;

		if(vCarLocal.x > 5 && wheel.longitudinalForce < 0)
			force.y = abs(wheel.longitudinalForce) * sin(wheel.steeringWheelAngle);
		else
			force.y = wheel.longitudinalForce * sin(wheel.steeringWheelAngle);
		resultForces.push_back({ wheel.position, force });

		force.x = - wheel.lateralForce * sin(wheel.steeringWheelAngle);
		force.y = + wheel.lateralForce * cos(wheel.steeringWheelAngle);
		resultForces.push_back({ wheel.position, force });
	}


	carDrifting = false;
	for (auto& wheel : allWheels)
	{
		if (vCarGlobal.length() > 5)
		{
			if (abs(wheel.slipAngle) > 15 || (abs(wheel.angularVelocity) / vCarGlobal.length()) < 0.8)
			{
				carDrifting = true;
				break;
			}
		}
	}


	return resultForces;
}

void PacejkaModel::calculateLocalVelocities(const vector2D& vCarGlobal, float angularVelocity, float rz)
{
	for (auto& wheel : allWheels)
	{
		vector2D R({0.0f, 0.0f}, wheel.position);
		vector2D rotateVelocity{-R.y, R.x};
		rotateVelocity.convertIntoUnitVector();
		rotateVelocity *= angularVelocity * R.length();

		vector2D globalVelocity = vCarGlobal;// +rotateVelocity;

		wheel.localVelocity = { globalVelocity.x * cos(-rz - wheel.steeringWheelAngle) - globalVelocity.y * sin(-rz - wheel.steeringWheelAngle), globalVelocity.x * sin(-rz - wheel.steeringWheelAngle) + globalVelocity.y * cos(-rz - wheel.steeringWheelAngle) };
		wheel.localVelocity += rotateVelocity;
	}
}

void PacejkaModel::calculateSlipRatios()
{
	for (auto& wheel : allWheels)
	{
		if (wheel.angularVelocity >= 0 && wheel.localVelocity.x < 0)
		{
			wheel.slipRatio = 100;
		}
		else if (wheel.angularVelocity <= 0 && wheel.localVelocity.x > 0)
		{
			wheel.slipRatio = -100;
		}
		else if (wheel.angularVelocity >= 0 && wheel.localVelocity.x >= 0)
		{
			if (abs(wheel.angularVelocity) > 0.1)
				wheel.slipRatio = (abs(wheel.angularVelocity) * rd / std::max(abs(wheel.localVelocity.x), 0.1) - 1.0f) * 100;
			else
				wheel.slipRatio = 0;
		}
		else if (wheel.angularVelocity <= 0 && wheel.localVelocity.x <= 0)
		{
			if (abs(wheel.angularVelocity) > 0.1)
				wheel.slipRatio = -(abs(wheel.angularVelocity) * rd / std::max(abs(wheel.localVelocity.x), 0.1) - 1.0f) * 100;
			else
				wheel.slipRatio = 0;
		}

		if (wheel.slipRatio > 100)
			wheel.slipRatio = 100;
		if (wheel.slipRatio < -100)
			wheel.slipRatio = -100;
	}
}

void PacejkaModel::calculateSlipAngles()
{
	for (auto& wheel : allWheels)
	{
		if (!wheel.localVelocity.x && !wheel.localVelocity.y)
			wheel.slipAngle = 0;
		else if (!wheel.localVelocity.x)
		{
			if(wheel.localVelocity.y > 0)
				wheel.slipAngle = -PI / 2;
			else
				wheel.slipAngle = PI / 2;
		}
		else
		{
			wheel.slipAngle = abs(atan(wheel.localVelocity.y / wheel.localVelocity.x));
			if (wheel.localVelocity.y > 0)
				wheel.slipAngle *= -1;
		}

		wheel.slipAngle *= 100 / (PI / 2);
	}
}

void PacejkaModel::calculateLongitudinalForces()
{
	for (auto& wheel : allWheels)
	{
		if(wheel.WD)
			wheel.longitudinalForce = mass / 4 * GRAVITY * D_Peak * sin(C_Shape_Longitudinal * atan(B_Stiffness * wheel.slipRatio - E_Curvature * (B_Stiffness * wheel.slipRatio - atan(B_Stiffness * wheel.slipRatio))));
	}
}

void PacejkaModel::calculateLateralForces()
{
	for (auto& wheel : allWheels)
	{
		wheel.lateralForce = mass / 4 * GRAVITY * D_Peak * sin(C_Shape_Lateral * atan(B_Stiffness * wheel.slipAngle - E_Curvature * (B_Stiffness * wheel.slipAngle - atan(B_Stiffness * wheel.slipAngle))));
	}
}

void PacejkaModel::recalculateWheelAngularVelocity(float v, bool allWheelsRecalculation)
{
	float drivingWheelVelocity = 0.0f;
	for (auto& wheel : allWheels)
	{
		if (!wheel.WD || allWheelsRecalculation)
			wheel.angularVelocity = v / rd;
		if (wheel.WD)
			drivingWheelVelocity = wheel.angularVelocity;
	}

	if (abs(v) < 0.5f)
	{
		for (auto& wheel : allWheels)
		{
			if (!wheel.WD)
				wheel.angularVelocity = drivingWheelVelocity;
		}
	}
}

void PacejkaModel::stop()
{
	for (auto& wheel : allWheels)
	{
		wheel.angularVelocity = 0;
	}

}