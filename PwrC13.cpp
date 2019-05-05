#include "PwrC13.h"

PwrC13::PwrC13(MapObject& mapObject) : Building(mapObject)
{
	_color = Color(ColorName::GRAY);
}

void PwrC13::generateWalls()
{
	Building::generateWalls();

	if (getId() == 353785323)
	{
		walls[1].xRatio = 2.0f;
		for (auto& wall : walls)
		{
			wall.textureName = Textures::PwrC13;
		}
	}
	else if (getId() == 479882766)
	{
		for (auto& wall : walls)
		{
			wall.textureName = Textures::PwrC13;
			wall.xRatio = 2.33f;
		}
	}
	else if (getId() == 479882765)
	{
		for (auto& wall : walls)
		{
			wall.textureName = Textures::office_windows;
			wall.yRatio = 0.135f;
		}
	}
}