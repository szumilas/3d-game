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
	float totalLength; //[m]
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
	SkodaOctavia,
	VolkswagenGolf,
	BMW3,
	OpelCorsa,
	CitroenC3,
	Peugeot208,
	SuzukiBaleno,
	SuzukiVitara,
	HondaJazz,
	ToyotaHilux,
	LamborghiniHuracan,
	RollsRoycePhantom,
	SubaruBRZ,
};

static std::map<CarBrand, CarDetails> carDB
{
	{ CarBrand::SkodaOctavia,{ "Skoda Octavia", "Data/Cars/skoda_octavia/", Textures::skoda_octavia, Sounds::engine, 4.66f, 1155.00f, 215.00f, 103.00f, 8.40f, 3.65f, std::vector<float>{3.78f, 2.12f, 1.36f, 1.03f, 0.86f, 0.73f}, 3.60f } }, // https://www.cars-data.com/en/skoda-octavia-1.4-tsi-140hp-greentech-elegance-specs/63421

	{ CarBrand::VolkswagenGolf,{ "Volkswagen Golf", "Data/Cars/volkswagen_golf/", Textures::volkswagen_golf, Sounds::engine, 4.27f, 1277.00f, 230.00f, 135.00f, 7.50f, 2.76f, std::vector<float>{3.77f, 1.96f, 1.26f, 0.87f, 0.86f, 0.72f}, 4.55f } }, // https://www.cars-data.com/en/volkswagen-golf-2.0-tdi-184hp-gtd-specs/79273

	{ CarBrand::BMW3,{ "BMW 3", "Data/Cars/bmw_3/", Textures::bmw_3, Sounds::engine, 4.53f, 1345.00f, 228.00f, 125.00f, 8.20f, 3.38f, std::vector<float>{4.32f, 2.46f, 1.66f, 1.23f, 1.00f, 0.85f}, 3.94f } }, // https://www.cars-data.com/en/bmw-325d-high-executive-specs/5927

	{ CarBrand::OpelCorsa,{ "Opel Corsa", "Data/Cars/opel_corsa/", Textures::opel_corsa, Sounds::engine, 4.04f, 1178.00f, 225.00f, 141.00f, 7.20f, 4.18f, std::vector<float>{3.82f, 2.16f, 1.48f, 1.07f, 0.88f, 0.74f}, 3.55f } }, // https://www.cars-data.com/en/opel-corsa-opc-specs/30535

	{ CarBrand::CitroenC3,{ "Citroen C3", "Data/Cars/citroen_c3/", Textures::citroen_c3, Sounds::engine, 3.97f, 1025.00f, 188.00f, 81.00f, 10.40f, 3.94f, std::vector<float>{3.55f, 1.90f, 1.23f, 0.86f, 0.69f}, 3.25f } }, // https://www.cars-data.com/en/citroen-c3-puretech-110-feel-specs/75421

	{ CarBrand::Peugeot208,{ "Peugeot 208", "Data/Cars/peugeot_208/", Textures::peugeot_208, Sounds::engine, 3.96f, 1050.00f, 190.00f, 85.00f, 9.70f, 3.41f, std::vector<float>{3.52f, 1.91f, 1.27f, 0.90f, 0.67f, 0.56f}, 3.58f } }, // https://www.cars-data.com/en/peugeot-208-active-1.6-e-hdi-115hp-specs/58889/tech
	{ CarBrand::SuzukiBaleno,{ "Suzuki Baleno", "Data/Cars/suzuki_baleno/", Textures::suzuki_baleno, Sounds::engine, 3.99f, 880.00f, 200.00f, 82.00f, 11.40f, 3.94f, std::vector<float>{3.55f, 1.90f, 1.23f, 0.86f, 0.69f}, 3.25f } }, // https://www.cars-data.com/en/suzuki-baleno-1.0-boosterjet-high-executive-specs/79037
	{ CarBrand::HondaJazz,{ "Honda Jazz", "Data/Cars/honda_jazz/", Textures::honda_jazz, Sounds::engine, 3.90f, 1051.00f, 175.00f, 73.00f, 12.60f, 3.94f, std::vector<float>{3.55f, 1.90f, 1.23f, 0.86f, 0.69f}, 3.25f } }, // https://www.cars-data.com/en/honda-jazz-1.4-lifestyle-specs/64705


	
	//{ CarBrand::ToyotaAvensisVerso, { 1395.0f, 192.0f, 110.0f, 3.94f, std::vector<float>{0.0f, 3.29f, 1.96f, 1.32f, 1.03f, 0.82f}}},
	{ CarBrand::LamborghiniHuracan, { "Lamborghini Huracan", "Data/Cars/lamborghini_huracan/", Textures::lamborghini_huracan, Sounds::engine, 4.46f, 1422.0f, 325.0f, 449.0f, 3.2f, 3.54f, std::vector<float>{0.0f, 3.91f, 2.44f, 1.81f, 1.46f, 1.18f, 0.97f, 0.84f}, 4.0f } }, //https://www.cars-data.com/en/lamborghini-huracan-lp610-4-specs/67416
	{ CarBrand::RollsRoycePhantom, { "Rolls-Royce Phantom", "Data/Cars/rolls_royce_phantom/", Textures::rolls_royce_phantom, Sounds::engine, 6.09f, 2565.0f, 250.0f, 338.0f, 6.1f, 3.15f, std::vector<float>{0.0f, 4.71f, 3.14f, 2.11f, 1.67f, 1.29f, 1.00f, 0.84f, 0.67f}, 3.32f } }, //https://www.cars-data.com/en/rolls-royce-phantom-ewb-specs/61335/tech
	{ CarBrand::ToyotaHilux,{ "Toyota Hilux", "Data/Cars/toyota_hilux/", Textures::toyota_hilux, Sounds::engine2, 5.26f, 1765.0f, 170.0f, 106.0f, 14.1f, 3.73f, std::vector<float>{0.0f, 4.32f, 2.33f, 1.44f, 1.00f, 0.79f}, 4.22f } }, //https://www.cars-data.com/en/toyota-hilux-xtra-cabine-2.5-d-4d-f-2wd-lx-specs/48548
	{ CarBrand::SuzukiVitara,{ "Suzuki Vitara", "Data/Cars/suzuki_vitara/", Textures::suzuki_vitara, Sounds::engine2, 4.18f, 1270.0f, 180.0f, 88.0f, 12.4f, 3.61f, std::vector<float>{0.0f, 3.54f, 2.05f, 1.23f, 0.88f, 0.70f, 0.59f}, 3.41f } }, //https://www.cars-data.com/en/suzuki-vitara-1.6-diesel-allgrip-limited-edition-specs/73063
	{ CarBrand::SubaruBRZ,{ "Subaru BRZ", "Data/Cars/subaru_brz/", Textures::subaru_brz, Sounds::engine2, 4.24f, 1205.0f, 226, 147.0f, 7.6f, 4.1f, std::vector<float>{0.0f, 3.63f, 2.19f, 1.54f, 1.21f, 1.00f, 0.77f}, 3.44f } }, //https://www.cars-data.com/en/subaru-brz-sport-premium-specs/74064
	
};