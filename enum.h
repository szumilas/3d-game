#pragma once

enum class Textures : long
{
	tenement_house_windows,
	tenement_house_no_windows,
	roof,
	roof_asphalt,
	apartment_windows,
	apartment_no_windows,
	apartment_one_window,
	office_windows,
	sedesowiec_front,
	sedesowiec_side,
	asphalt,
	grass,
	paving,
	skybox,
	sett,
	rail,
	bollard,
	concrete_wall,
	fence,
	guard_rail,
	hedge,
	retaining_wall,
	water,
	bus_shelter,
	bus_shelter_with_sign,
	crossing,
	pasaz_grunwaldzki,
	pasaz_grunwaldzki_tunnel,
	pasaz_grunwaldzki_banner,
	pasaz_grunwaldzki_single,
	pasaz_grunwaldzki_front,
	glass_elevation,
	grunwaldzki,
	PwrC13,
	building_00_big,
	building_00_small,
	building_00_empty,
	building_01_big,
	building_01_small,
	building_01_empty,
	building_02_big,
	building_02_small,
	building_02_empty,
	building_03_big,
	building_03_small,
	building_03_empty,
	building_04_big,
	building_04_small,
	building_04_empty,
	building_05_big,
	building_05_small,
	building_05_empty,
	building_06_big,
	building_06_small,
	building_06_empty,
	building_07_big,
	building_07_small,
	building_07_empty,
	skoda_octavia,
	toyota_hilux,
	lamborghini_huracan,
	toyota_yaris,
	suzuki_vitara,
	subaru_brz,
	rolls_royce_phantom,
	alfa_romeo_147,
	audi_r8_v10_coupe,
	car_gauge,
	car_gauge_330,
	elm_tree,
	no_texture,
	map_editor_panel,
};

enum class Sounds : long
{
	engine,
	engine2,
	drift,
};

enum class Exceptions : long
{
	ERR_WHILE_CONVERTING_IMAGE,
	ERR_WHILE_LOADING_IMAGE,
	ERR_WHILE_INIT_ALLEGRO,
	ERR_WHILE_INIT_SOUND,
	ERR_WHILE_CREATE_ALLEGRO_VOICE,
	ERR_WHILE_CREATE_ALLEGRO_MIXER,
	ERR_WHILE_ATTACHING_MIXER_TO_VOICE,
	ERR_WHILE_LOADING_SAMPLE,
	ERR_WHILE_CREATE_SAMPLE,
	ERR_WHILE_SET_SAMPLE_PTR,
	ERR_WHILE_ATTACHING_SAMPLE_INSTANCE_TO_MIXER,
	ERR_WHILE_ATTACHING_SUBMIXER_TO_MIXER,

};

enum class ColorName : long
{
	WHITE,
	BLACK,
	LIGHT_GRAY,
	DARK_GRAY,
	GRAY,
	RED,
	GREEN,
	BLUE,
	ORANGE,
	BROWN,
	YELLOW,
	PINK,
};

enum
{
	frontLeftWheel,
	frontRightWheel,
	rearLeftWheel,
	rearRightWheel,
};

enum class Collidable : long
{
	no,
	point,
	polygon,
};