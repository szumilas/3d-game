#pragma once

#include <iostream>
#include <map>
#include <vector>

#include "enum.h"

struct CarDetails
{
	std::string name;
	std::string objFilePath;
	Textures textureName;
	Sounds engineSound;
	float backWheelsXoffset; //[m]
	float frontWheelsXoffset; //[m]
	float frontWheelsYoffset; //[m]
	float mass; //[kg]
	float vMax; //[km / h]
	float power; //[kW]
	float acceleration_0_100; //[s]
	float mainTransmission; //[-]
	std::vector<float> gears; //[-]
	float reverseGear;
};

enum class CarBrand : long
{
	ToyotaHilux,
	LamborghiniHuracan,
	RollsRoycePhantom,
	SuzukiVitara,
	SubaruBRZ,
};

static std::map<CarBrand, CarDetails> carDB
{
	//{ CarBrand::ToyotaAvensisVerso, { 1395.0f, 192.0f, 110.0f, 3.94f, std::vector<float>{0.0f, 3.29f, 1.96f, 1.32f, 1.03f, 0.82f}}},
	{ CarBrand::LamborghiniHuracan, { "Lamborghini Huracan", "Data/Cars/lamborghini_huracan/", Textures::lamborghini_huracan, Sounds::engine, -1.45f, 1.27f, 0.85f, 1422.0f, 325.0f, 449.0f, 3.2f, 3.54f, std::vector<float>{0.0f, 3.91f, 2.44f, 1.81f, 1.46f, 1.18f, 0.97f, 0.84f}, 4.0f } }, //https://www.cars-data.com/en/lamborghini-huracan-lp610-4-specs/67416
	{ CarBrand::RollsRoycePhantom, { "Rolls-Royce Phantom", "Data/Cars/rolls_royce_phantom/", Textures::rolls_royce_phantom, Sounds::engine, -1.50f, 2.00f, 0.77f, 2565.0f, 250.0f, 338.0f, 6.1f, 3.15f, std::vector<float>{0.0f, 4.71f, 3.14f, 2.11f, 1.67f, 1.29f, 1.00f, 0.84f, 0.67f}, 3.32f } }, //https://www.cars-data.com/en/rolls-royce-phantom-ewb-specs/61335/tech
	{ CarBrand::ToyotaHilux,{ "Toyota Hilux", "Data/Cars/toyota_hilux/", Textures::toyota_hilux, Sounds::engine2, -1.45f, 1.78f, 0.72f, 1765.0f, 170.0f, 106.0f, 14.1f, 3.73f, std::vector<float>{0.0f, 4.32f, 2.33f, 1.44f, 1.00f, 0.79f}, 4.22f } }, //https://www.cars-data.com/en/toyota-hilux-xtra-cabine-2.5-d-4d-f-2wd-lx-specs/48548
	{ CarBrand::SuzukiVitara,{ "Suzuki Vitara", "Data/Cars/suzuki_vitara/", Textures::suzuki_vitara, Sounds::engine2, -1.45f, 1.32f, 0.77f, 1270.0f, 180.0f, 88.0f, 12.4f, 3.61f, std::vector<float>{0.0f, 3.54f, 2.05f, 1.23f, 0.88f, 0.70f, 0.59f}, 3.41f } }, //https://www.cars-data.com/en/suzuki-vitara-1.6-diesel-allgrip-limited-edition-specs/73063
	{ CarBrand::SubaruBRZ,{ "Subaru BRZ", "Data/Cars/subaru_brz/", Textures::subaru_brz, Sounds::engine2, -1.45f, 1.32f, 0.77f, 1205.0f, 226, 147.0f, 7.6f, 4.1f, std::vector<float>{0.0f, 3.63f, 2.19f, 1.54f, 1.21f, 1.00f, 0.77f}, 3.44f } }, //https://www.cars-data.com/en/subaru-brz-sport-premium-specs/74064

};