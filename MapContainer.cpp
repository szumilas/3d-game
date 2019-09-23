#include "MapContainer.h"
#include <fstream>

std::vector<std::vector<std::vector<std::unique_ptr<MapObject>*>>> MapContainer::mapObjectSections;
std::vector<std::vector<std::vector<std::unique_ptr<MapObject>*>>> MapContainer::mapCollidableObjectSections;
std::unique_ptr<MapObject>* MapContainer::background;
float MapContainer::deltaX;
float MapContainer::deltaY;
float MapContainer::maxX;
float MapContainer::minX;
float MapContainer::maxY;
float MapContainer::minY;
std::unique_ptr<MapContainer> MapContainer::_instance;
std::vector<Car> MapContainer::cars;
std::vector<MapContainer::AIPointStruct> MapContainer::AIPoints;
std::vector<float> MapContainer::AIPointsDistances;
RaceTimer MapContainer::raceTimer;
int MapContainer::w;
int MapContainer::h;
int MapContainer::mapEditorSelectedPanel = 0;
int MapContainer::mapEditorSelectedTool = 0;
int MapContainer::currentToolId = MapContainer::AddPoint;
std::vector<std::vector<int>> MapContainer::tools = MapContainer::createTools();
std::map<int, void (MapContainer::*)(const Point&)> MapContainer::toolsMap = createToolsMap();

std::vector<std::vector<int>> MapContainer::createTools()
{
	std::vector<std::vector<int>> tools;

	std::vector<int> AITools{
		MapContainer::AddPoint,
		MapContainer::RemovePoint,
		MapContainer::MovePoint,
		MapContainer::SelectPoint,
		MapContainer::SaveAIPath,
		MapContainer::LoadAIPath,
		MapContainer::AIPlay,
		MapContainer::AIPause,
		MapContainer::AIStop,
		MapContainer::AIStopAndResumePosition,
	};

	tools.push_back(AITools);
	tools.push_back({});
	tools.push_back({});

	return tools;
}

std::map<int, void (MapContainer::*)(const Point&)> MapContainer::createToolsMap()
{
	std::map<int, void (MapContainer::*)(const Point&)> map = {
	
		{ AddPoint, &MapContainer::addAIPoint },
		{ RemovePoint, &MapContainer::removeAIPoint },
		{ MovePoint, &MapContainer::moveAIPoint },
		{ SelectPoint, &MapContainer::selectAIPoint },
		{ SaveAIPath, &MapContainer::SaveAIPoints },
		{ LoadAIPath, &MapContainer::LoadAIPoints },
		{ AIPlay, &MapContainer::setAIPathActive },
		{ AIPause, &MapContainer::pauseAllCars },
		{ AIStop, &MapContainer::stopAllCars },
		{ AIStopAndResumePosition, &MapContainer::stopAllCarsToSelectedPoint },
	
	};

	return map;
}




void MapContainer::Init(int w, int h)
{
	MapContainer::w = w;
	MapContainer::h = h;
	_instance = std::unique_ptr<MapContainer>(new MapContainer());
}

std::unique_ptr<MapContainer>& MapContainer::Instance()
{
	return _instance;
}

std::vector<std::vector<std::unique_ptr<MapObject>*>*> MapContainer::getCollidableObjectsInPosition(const Point& position)
{
	std::vector<std::vector<std::unique_ptr<MapObject>*>*> result;

	int yToReturn = static_cast<int>(100 * (position.y - minY) / deltaY);
	int xToReturn = static_cast<int>(100 * (position.x - minX) / deltaX);

	if (yToReturn >= 100 || xToReturn >= 100 || yToReturn < 0 || xToReturn < 0)
		return result;

	result.push_back(&mapCollidableObjectSections[xToReturn][yToReturn]);
	if(yToReturn > 0)
		result.push_back(&mapCollidableObjectSections[xToReturn][yToReturn - 1]);
	if (yToReturn < 99)
		result.push_back(&mapCollidableObjectSections[xToReturn][yToReturn + 1]);
	if (xToReturn > 0)
		result.push_back(&mapCollidableObjectSections[xToReturn - 1][yToReturn]);
	if (xToReturn < 99)
		result.push_back(&mapCollidableObjectSections[xToReturn + 1][yToReturn]);

	if (xToReturn > 0 && yToReturn > 0)
		result.push_back(&mapCollidableObjectSections[xToReturn - 1][yToReturn - 1]);
	if (xToReturn > 0 && yToReturn < 99)
		result.push_back(&mapCollidableObjectSections[xToReturn - 1][yToReturn + 1]);
	if (xToReturn < 99 && yToReturn > 0)
		result.push_back(&mapCollidableObjectSections[xToReturn + 1][yToReturn - 1]);
	if (xToReturn < 99 && yToReturn < 99)
		result.push_back(&mapCollidableObjectSections[xToReturn + 1][yToReturn + 1]);

	return result;
}

void MapContainer::displayWorld(std::pair<Point, Point>& camera)
{
	//Point& center = cars[0].getCameraCenter();
	//Point& lookAt = cars[0].getCameraLookAt();

	Point& center = camera.first;
	Point& lookAt = camera.second;

	displayLines();


	int yToDraw = static_cast<int>(100 * (center.y - minY) / deltaY);
	int xToDraw = static_cast<int>(100 * (center.x - minX) / deltaX);

	if (yToDraw >= 100 || xToDraw >= 100)
		return;
	
	std::vector<std::pair<int, int>> sectionsToDisplay;

	sectionsToDisplay.push_back({ xToDraw, yToDraw });

	vector2D frontRay(center, lookAt);
	frontRay.convertIntoUnitVector();
	frontRay.rotate(PI / 3);

	for (int i = 0; i <= 48; i++)
	{
		int distanceUnit = 5;
		bool rayFinished = false;

		for(; distanceUnit < 1500; distanceUnit += 5)
		{
			Point currenPoint(center.x + frontRay.x * distanceUnit, center.y + frontRay.y * distanceUnit);
			int sectionX = static_cast<int>(100 * (currenPoint.x - minX) / deltaX);
			int sectionY = static_cast<int>(100 * (currenPoint.y - minY) / deltaY);

			if (sectionX >= 100 || sectionY >= 100 ||
				sectionX < 0 || sectionY < 0)
			{
				break;
			}
			else
			{
				sectionsToDisplay.push_back({ sectionX, sectionY });
			}
			
			for (auto& object : mapObjectSections[sectionsToDisplay.back().first][sectionsToDisplay.back().second])
			{
				if (!object->get()->building.empty() && object->get()->_min_height == 0 && object->get()->_height >= 10)
				{
					if (PointInsidePolygonDetector::isInside(object->get()->points, currenPoint))
					{
						rayFinished = true;
						break;
					}
				}
			}

			if (rayFinished)
				break;
		}

		/*glBegin(GL_LINES);
		glColor3f(0.8f, 0.2f, 0.8f);
		glVertex3f(center.x, center.y, 2.0f);
		glVertex3f(center.x + frontRay.x * distanceUnit, center.y + frontRay.y * distanceUnit, 2.0f);
		glEnd();*/

		frontRay.rotate(- PI / 72);
	}

	sort(sectionsToDisplay.begin(), sectionsToDisplay.end());
	sectionsToDisplay.erase(unique(sectionsToDisplay.begin(), sectionsToDisplay.end()), sectionsToDisplay.end());

	for (auto& sectionToDisplay : sectionsToDisplay)
	{
		if(sectionToDisplay.first < 100 && sectionToDisplay.second < 100 &&
		   sectionToDisplay.first >= 0 && sectionToDisplay.second >= 0)
		for (auto& object : mapObjectSections[sectionToDisplay.first][sectionToDisplay.second])
		{
			if(!object->get()->isHidden)
				object->get()->display();
		}
	}

	for (auto& sectionToDisplay : sectionsToDisplay)
	{
		for (auto& object : mapObjectSections[sectionToDisplay.first][sectionToDisplay.second])
		{
			object->get()->alreadyPrinted = false;
		}
	}
	
	displayBackground();
	displayAIPoints();
}

void MapContainer::loadWorldIntoSections(std::vector<std::unique_ptr<MapObject>>& mapObjects)
{
	mapObjectSections.resize(100);
	mapCollidableObjectSections.resize(100);

	for (auto& row : mapObjectSections)
	{
		row.resize(100);
	}
	for (auto& row : mapCollidableObjectSections)
	{
		row.resize(100);
	}

	maxX = -1000000.0f;
	minX = 1000000.0f;
	maxY = -1000000.0f;
	minY = 1000000.0f;

	for (auto& mapObject : mapObjects)
	{
		for (auto& polygon : mapObject->polygons)
		{
			for (auto& point : polygon.points)
			{
				if (point.x < minX)
					minX = point.x;
				if (point.x > maxX)
					maxX = point.x;
				if (point.y < minY)
					minY = point.y;
				if (point.y > maxY)
					maxY = point.y;
			}
		}
	}

	maxX += 0.01;
	maxY += 0.01;

	deltaX = maxX - minX;
	deltaY = maxY - minY;

	for (auto& mapObject : mapObjects)
	{
		std::vector<std::pair<int, int>> sectionsXY;

		for (auto& polygon : mapObject->polygons)
		{
			for (auto& point : polygon.points)
			{
				sectionsXY.push_back({(point.x - minX) * 100 / deltaX, (point.y - minY) * 100 / deltaY});
			}
		}

		sort(sectionsXY.begin(), sectionsXY.end());
		sectionsXY.erase(unique(sectionsXY.begin(), sectionsXY.end()), sectionsXY.end());

		for (auto& sectionXY : sectionsXY)
		{
			mapObjectSections[sectionXY.first][sectionXY.second].push_back(&mapObject);
			if (mapObject->collidable != Collidable::no)
			{
				mapCollidableObjectSections[sectionXY.first][sectionXY.second].push_back(&mapObject);
			}
		}

	}
	
	if(!mapObjects.empty())
		background = &mapObjects.back();

}

void MapContainer::displayAllWorld()
{
	for (auto& lineToDisplay : mapObjectSections)
	{
		for (auto& sectionToDisplay : lineToDisplay)
		{
			for (auto& object : sectionToDisplay)
			{
				object->get()->display();
				object->get()->alreadyPrinted = false;
			}
		}
	}

	displayBackground();
	displayAIPoints();
}

void MapContainer::displaySector(const Point& point)
{
	int yToDraw = static_cast<int>(100 * (point.y - minY) / deltaY);
	int xToDraw = static_cast<int>(100 * (point.x - minX) / deltaX);

	std::vector<std::pair<int, int>> sectorsToDisplay;
	for (int q = -5; q <= 5; q++)
	{
		for (int w = -5; w <= 5; w++)
		{
			if(xToDraw + q >= 0 && xToDraw + q < 100 && yToDraw + w >= 0 && yToDraw + w < 100)
				sectorsToDisplay.push_back({ xToDraw + q, yToDraw + w });
		}
	}

	for (auto& sectionToDisplay : sectorsToDisplay)
	{
		if (sectionToDisplay.first < 100 && sectionToDisplay.second < 100 &&
			sectionToDisplay.first >= 0 && sectionToDisplay.second >= 0)
			for (auto& object : mapObjectSections[sectionToDisplay.first][sectionToDisplay.second])
			{
				if (!object->get()->isHidden)
					object->get()->display();
			}
	}

	for (auto& sectionToDisplay : sectorsToDisplay)
	{
		for (auto& object : mapObjectSections[sectionToDisplay.first][sectionToDisplay.second])
		{
			object->get()->alreadyPrinted = false;
		}
	}

	displayLines();
	displayBackground();
	displayAIPoints();
}

void MapContainer::displayBackground()
{
	if (background != nullptr)
	{
		background->get()->display();
		background->get()->alreadyPrinted = false;
	}
}

void MapContainer::displayLines()
{
	glColor3f(0.0f, 0.0f, 0.0f);
	for (int xx = 0; xx < 100; xx++)
	{
		glBegin(GL_LINES);
		glVertex3f(minX + xx * deltaX / 100, minY, 0.05);
		glVertex3f(minX + xx * deltaX / 100, maxY, 0.05);
		glEnd();
	}
	for (int yy = 0; yy < 100; yy++)
	{
		glBegin(GL_LINES);
		glVertex3f(minX, minY + yy * deltaY / 100, 0.05);
		glVertex3f(maxX, minY + yy * deltaY / 100, 0.05);
		glEnd();
	}
}

void MapContainer::displayMapEditorPanel()
{

	glEnable(GL_BLEND);

	glLineWidth(3);

	glBegin(GL_LINE_LOOP);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(0.5 * w - 0.5 * h + mapEditorSelectedPanel * 0.05 * h, 0.95 * h);
	glVertex2f(0.5 * w - 0.5 * h + (mapEditorSelectedPanel + 1) * 0.05 * h, 0.95 * h);
	glVertex2f(0.5 * w - 0.5 * h + (mapEditorSelectedPanel + 1) * 0.05 * h, h);
	glVertex2f(0.5 * w - 0.5 * h + mapEditorSelectedPanel * 0.05 * h, h);

	glEnd();

	glBegin(GL_LINE_LOOP);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(0.5 * w - 0.5 * h + mapEditorSelectedTool * 0.05 * h, 0.9 * h);
	glVertex2f(0.5 * w - 0.5 * h + (mapEditorSelectedTool + 1) * 0.05 * h, 0.9 * h);
	glVertex2f(0.5 * w - 0.5 * h + (mapEditorSelectedTool + 1) * 0.05 * h, 0.95 * h);
	glVertex2f(0.5 * w - 0.5 * h + mapEditorSelectedTool * 0.05 * h, 0.95 * h);

	glEnd();

	glLineWidth(1.0);

	glDisable(GL_BLEND);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	
	static int idTexture = TextureManager::Instance()->textures[static_cast<int>(Textures::map_editor_panel)].idTexture;

	glBindTexture(GL_TEXTURE_2D, idTexture);
	glBegin(GL_POLYGON);

	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0, static_cast<float>(tools.size()) / (tools.size() + 1));
	glVertex2f(0.5 * w - 0.5 * h, 0.95 * h);
	glTexCoord2f(1, static_cast<float>(tools.size()) / (tools.size() + 1));
	glVertex2f(0.5 * w + 0.5 * h, 0.95 * h);
	glTexCoord2f(1, 1);
	glVertex2f(0.5 * w + 0.5 * h, h);
	glTexCoord2f(0, 1);
	glVertex2f(0.5 * w - 0.5 * h, h);

	glEnd();


	glBegin(GL_POLYGON);

	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0, static_cast<float>(tools.size() - 1 - mapEditorSelectedPanel) / (tools.size() + 1));
	glVertex2f(0.5 * w - 0.5 * h, 0.9 * h);
	glTexCoord2f(1, static_cast<float>(tools.size() - 1 - mapEditorSelectedPanel) / (tools.size() + 1));
	glVertex2f(0.5 * w + 0.5 * h, 0.9 * h);
	glTexCoord2f(1, static_cast<float>(tools.size() - mapEditorSelectedPanel) / (tools.size() + 1));
	glVertex2f(0.5 * w + 0.5 * h, 0.95 * h);
	glTexCoord2f(0, static_cast<float>(tools.size() - mapEditorSelectedPanel) / (tools.size() + 1));
	glVertex2f(0.5 * w - 0.5 * h, 0.95 * h);

	glEnd();


	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void MapContainer::pickTool(float pX, float pY)
{
	if (pY >= 0.95 && pX >= -0.5 && pX <= 0.5)
	{
		auto nextSelectedPanel = (pX + 0.5) * 20;
		if (tools.size() >= nextSelectedPanel)
			mapEditorSelectedPanel = nextSelectedPanel;

		mapEditorSelectedTool = 0;
	}
	else if (pY >= 0.9 && pY <= 0.95 && pX >= -0.5 && pX <= 0.5)
	{
		mapEditorSelectedTool = (pX + 0.5) * 20;
	}

	if (tools[mapEditorSelectedPanel].size() > mapEditorSelectedTool)
	{
		currentToolId = tools[mapEditorSelectedPanel][mapEditorSelectedTool];
	}
}
void MapContainer::useTool(const Point& point)
{
	auto toolIt = toolsMap.find(currentToolId);
	if (toolIt != toolsMap.end())
	{
		auto tool = toolIt->second;
		(Instance().get()->*tool)(point);
	}

}

void MapContainer::recalculateAIPointsDistances()
{
	AIPointsDistances.clear();
	if (AIPoints.size() <= 1)
	{
		AIPointsDistances.push_back(0.0f);
	}
	else
	{
		for (int q = 0; q < AIPoints.size(); q++)
		{
			AIPointsDistances.push_back(AIPoints[q].center.distance2D(AIPoints[(q + 1) % AIPoints.size()].center));
		}
	}
}

void MapContainer::addAIPoint(const Point& point)
{
	AIPoints.push_back({ point, Color(ColorName::BLUE), false });
	recalculateAIPointsDistances();
}

void MapContainer::removeAIPoint(const Point& point)
{
	for (int q = 0; q < AIPoints.size(); q++)
	{
		if (AIPoints[q].selected)
		{
			AIPoints.erase(AIPoints.begin() + q);
			break;
		}
	}
}

void MapContainer::resetCarPositionsToPoint(int idPoint)
{
	vector2D direction(AIPoints[idPoint + 1].center, AIPoints[idPoint].center);
	direction.convertIntoUnitVector();
	float newRz = vector2D::directedAngle(vector2D({ 0, 0 }, { 1, 0 }), direction) + PI;

	vector2D perpendicular(direction);
	perpendicular.rotate(PI / 2);

	Point firstCarPosition = AIPoints[idPoint].center;

	int index = 0;
	for (auto& car : MapContainer::Instance()->cars)
	{
		Point carPosition = firstCarPosition;
		carPosition.move2D(index * 5 * direction.x, index * 5 * direction.y);
		carPosition.move2D(((index % 2) * 2 - 1) * perpendicular.x * 2, ((index % 2) * 2 - 1) * perpendicular.y * 2);
		car.setPosition(carPosition, newRz);
		car.setAIcurrentPoint(idPoint);
		index++;
	}

}

void MapContainer::stopAllCars(const Point& point)
{
	pauseAllCars();

	if (AIPoints.size() > 1)
	{
		resetCarPositionsToPoint(0);
	}
}

void MapContainer::stopAllCarsToSelectedPoint(const Point& point)
{
	pauseAllCars();

	int selectedPoint = getSelectedPointIndex();

	if (AIPoints.size() > selectedPoint + 1)
	{
		resetCarPositionsToPoint(selectedPoint);
	}
}

int MapContainer::getSelectedPointIndex()
{
	int index = 0;
	for (auto& AIPoint : AIPoints)
	{
		if (AIPoint.selected)
		{
			return index;
		}
		index++;
	}

	return 0;
}

void MapContainer::selectAIPoint(const Point& point)
{
	for (auto& AIPoint : AIPoints)
	{
		AIPoint.selected = false;
	}

	for (auto& AIPoint : AIPoints)
	{
		if (AIPoint.center.distance2D(point) < 2.0)
		{
			AIPoint.selected = true;
			break;
		}
	}
}

void MapContainer::moveAIPoint(const Point& point)
{
	for (auto& AIPoint : AIPoints)
	{
		if (AIPoint.selected == true)
		{
			AIPoint.center = point;
			break;
		}
	}
}

void MapContainer::pauseAllCars(const Point& point)
{
	AIPathActive = false;
	for (auto& car : MapContainer::Instance()->cars)
		car.stop();
}

void MapContainer::removeAIPoints()
{
	AIPoints.clear();
	recalculateAIPointsDistances();
}

void MapContainer::displayAIPoints()
{
	glPointSize(10.0);
	for (auto& AIPoint : AIPoints)
	{
		if (AIPoint.selected == true)
			glPointSize(15.0);
		
		glColor3f(AIPoint.color.red, AIPoint.color.green, AIPoint.color.blue);

		glBegin(GL_POINTS);
		glVertex3f(AIPoint.center.x, AIPoint.center.y, AIPoint.center.z);
		glEnd();
		glPointSize(10.0);
	}
	glPointSize(1.0);

	glColor3f(0.0f, 0.0f, 1.0f);
	for (int q = 0; q < static_cast<int>(AIPoints.size()) - 1; q++)
	{
		glBegin(GL_LINES);
		glVertex3f(AIPoints[q].center.x, AIPoints[q].center.y, 0.05);
		glVertex3f(AIPoints[q + 1].center.x, AIPoints[q + 1].center.y, 0.05);
		glEnd();
	}
}

void MapContainer::SetFuturePoints(const int& futurePoint)
{
	AIPoints[futurePoint].color = Color(ColorName::PINK);
}

void MapContainer::ClearFuturePoints()
{
	for (auto& AIPoint : AIPoints)
	{
		AIPoint.color = Color(ColorName::BLUE);
	}
}

void MapContainer::SaveAIPoints(const Point& point)
{
	std::ofstream file;
	file.open("Data/AIPoints.txt", std::ios::out);

	Point previous(-1000000, -100000);

	for (auto& AIPoint : AIPoints)
	{
		if (AIPoint.center.distance2D(previous) > 1)
		{
			file << AIPoint.center.x << " " << AIPoint.center.y << " " << AIPoint.center.z << "\n";
			previous = AIPoint.center;
		}
	}

	file.close();
}

void MapContainer::LoadAIPoints(const Point& point)
{
	std::ifstream file;
	file.open("Data/AIPoints.txt", std::ios::out);

	if (file)
	{
		removeAIPoints();
		float x, y, z;
		while (file >> x >> y >> z)
		{
			Point newPoint(x, y, z);
			AIPoints.push_back({ newPoint, Color(ColorName::BLUE), false });
		}
		recalculateAIPointsDistances();
	}

	file.close();
}

void MapContainer::initRaceTimer()
{
	raceTimer.init(&cars);
}

void MapContainer::updateRaceTimer()
{


}

void MapContainer::displayRaceTimer()
{
	raceTimer.display();

}