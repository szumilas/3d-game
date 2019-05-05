#include "Sedesowiec.h"

Sedesowiec::Sedesowiec(MapObject& mapObject) : Building(mapObject)
{
	_color = Color(ColorName::LIGHT_GRAY);
	epsilon = 0.5;
}

void Sedesowiec::generateWalls()
{
	Building::generateWalls();

	if (getId() == 101212302 || getId() == 402873616 || getId() == 403442403 || getId() == 440403931 || getId() == 440403932 || getId() == 440403933)
	{
		for (auto& wall : walls)
		{
			if (wall.wallLenght > 20)
			{
				wall.textureName = Textures::sedesowiec_front;
			}
			else
			{
				wall.textureName = Textures::sedesowiec_side;
			}
		}
	}
	else
	{
		for (auto& wall : walls)
		{
			wall.textureName = Textures::no_texture;
		}
	}
}