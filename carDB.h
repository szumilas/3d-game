#pragma once

#include <iostream>
#include <map>
#include <vector>

#include "enum.h"

struct CarDetails
{
	std::string objFilePath;
	Textures textureName;
	float mass; //[kg]
	float vMax; //[km / h]
	float power; //[kW]
	float mainTransmission; //[-]
	std::vector<float> gears; //[-]
};

enum class CarBrand : long
{
	ToyotaHilux,
	LamborghiniHuracan,
	RollsRoycePhantom,
	SuzukiVitara,
};

static std::map<CarBrand, CarDetails> carDB
{
	//{ CarBrand::ToyotaAvensisVerso, { 1395.0f, 192.0f, 110.0f, 3.94f, std::vector<float>{0.0f, 3.29f, 1.96f, 1.32f, 1.03f, 0.82f}}},
	{ CarBrand::LamborghiniHuracan, { "Data/Cars/lamborghini_huracan.obj", Textures::lamborghini_huracan, 1422.0f, 325.0f, 449.0f, 3.54f, std::vector<float>{0.0f, 3.91f, 2.44f, 1.81f, 1.46f, 1.18f, 0.97f, 0.84f} } },
	{ CarBrand::RollsRoycePhantom, { "Data/Cars/rolls_royce_phantom.obj", Textures::rolls_royce_phantom, 2565.0f, 250.0f, 338.0f, 3.15f, std::vector<float>{0.0f, 4.71f, 3.14f, 2.11f, 1.67f, 1.29f, 1.00f, 0.84f, 0.67f} } },
	{ CarBrand::ToyotaHilux,{ "Data/Cars/toyota_hilux.obj", Textures::toyota_hilux, 1765.0f, 170.0f, 106.0f, 3.73f, std::vector<float>{0.0f, 4.32f, 2.33f, 1.44f, 1.00f, 0.79f} } },
	{ CarBrand::SuzukiVitara,{ "Data/Cars/suzuki_vitara/", Textures::suzuki_vitara, 1270.0f, 180.0f, 88.0f, 3.61f, std::vector<float>{0.0f, 3.54f, 2.05f, 1.23f, 0.88f, 0.70f, 0.59f} } },


};