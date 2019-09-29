#include "MetaLine.h"

MetaLine::MetaLine(MapObject& mapObject) : Crossing(mapObject)
{
	if (points.size() == 2)
	{
		vector2D v(points.front(), points.back());
		int roundedLength = static_cast<int>(v.length());
		v.convertIntoUnitVector();
		v *= roundedLength;
		points.back().x = points.front().x + v.x;
		points.back().y = points.front().y + v.y;
	}

	gauge = 2.0f;
	textureName = Textures::meta;

	MapObject::createBothRailsForSymmetryAxis(points, finalLeftRail, finalRightRail, gauge);
	Crossing::calculateFinalGeometry();
	adjustTextures();
};

void MetaLine::adjustTextures()
{
	if (!polygons.empty() && polygons.front().texturePoints.size() == 4)
	{
		auto firstPoint = polygons.front().texturePoints[0];

		for (auto& point : polygons.front().texturePoints)
		{
			point.x -= firstPoint.x;
			point.y -= firstPoint.y;
		}
	}
}

std::pair<Point, Point> MetaLine::getMetaLinePoints()
{
	return { points.front(), points.back() };
}