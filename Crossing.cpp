#include "Crossing.h"

Crossing::Crossing(MapObject& mapObject) : Footway(mapObject)
{
	_min_height = 0.05f;
	Footway::gauge = 5.0f;

	textureName = Textures::crossing;
	drawingPriority = DrawingPriority::notImportant;
};

void Crossing::calculateFinalGeometry()
{
	auto& firstPoint = points[0];
	auto& lastPoint = points.back();

	vector2D v1(firstPoint, lastPoint);
	vector2D equator(Point(0.0, 0.0), Point(1.0, 0.0));

	auto angle = vector2D::realAngle(v1, equator);

	angle *= -1;
	angle += PI / 2;

	Footway::calculateFinalGeometry();

	for (auto& polygon : polygons)
	{
		for (auto& point : polygon.texturePoints)
		{
			Point pointCopy = point;
			point.x = pointCopy.x * cos(angle) - pointCopy.y * sin(angle);
			point.y = pointCopy.x * sin(angle) + pointCopy.y * cos(angle);
		}

	}

}