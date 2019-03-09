#pragma once

#include <iostream>
#include <map>
#include <vector>

struct CarDetails
{
	float mass; //[kg]
	float vMax; //[km / h]
	float power; //[kW]
	float mainTransmission; //[-]
	std::vector<float> gears; //[-]
};

enum class CarBrand : long
{
	ToyotaAvensisVerso,
	LamborghiniAventador,

};

static std::map<CarBrand, CarDetails> carDB
{
	{ CarBrand::ToyotaAvensisVerso, { 1395.0f, 192.0f, 110.0f, 3.94f, std::vector<float>{0.0f, 3.29f, 1.96f, 1.32f, 1.03f, 0.82f}}},
	{ CarBrand::LamborghiniAventador, { 1575, 350.0f, 515.0f, 3.54f, std::vector<float>{0.0f, 3.91f, 2.44f, 1.81f, 1.46f, 1.18f, 0.97f, 0.84f} } }


};