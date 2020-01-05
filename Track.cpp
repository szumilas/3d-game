#include "Track.h"

#include "MapManager.h"

#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>

const std::string currentDateTime()
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%Y.%m.%d-%X", &tstruct);

	return buf;
}

Track::Track(TrackName trackName) : trackName(trackName)
{
	_color = Color(ColorName::YELLOW);
	textureName = Textures::no_texture;

	auto fileName = "Track_" + (std::to_string(100 + static_cast<int>(trackName))).substr(1, 2) + ".txt";

	std::ifstream file;
	file.open("Data/" + fileName, std::ios::out);

	if (file)
	{
		AIPoints.clear();
		barrierPoints.clear();
		char type;
		while (file >> type)
		{
			if (type == 'p')
			{
				float x, y, z;
				file >> x >> y >> z;
				Point newPoint(x, y, z);
				AIPoints.push_back(newPoint);
			}
			else if (type == 'b')
			{
				float x1, y1, z1;
				float x2, y2, z2;
				file >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
				Point newPoint1(x1, y1, z1);
				Point newPoint2(x2, y2, z2);
				barrierPoints.push_back({ newPoint1, newPoint2 });
			}
			else if (type == 'm')
			{
				float x1, y1, z1;
				float x2, y2, z2;
				file >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
				Point newPoint1(x1, y1, z1);
				Point newPoint2(x2, y2, z2);
				meta = { newPoint1, newPoint2 };
			}
			else if (type == 'r')
			{
				int time;
				file >> time;
				lapRecord = time;
			}
		}

		calculateCoordinates();
		calculateLength();
	}

	file.close();
}

void Track::calculateCoordinates()
{
	static auto convertCoordinates = [](Point& p)
	{
		double longitude = (p.x / MapManager::longituteRatio) + MapManager::minLon;
		double latitude = (p.y / MapManager::latitudeRatio) + MapManager::minLat;
		return Point(longitude, latitude);
	};

	for (auto& AIPoint : AIPoints)
	{
		AIPointsCoordinates.push_back(convertCoordinates(AIPoint));
	}

	auto AIPointsScaled = AIPoints;
	auto maxX = std::max_element(AIPointsScaled.begin(), AIPointsScaled.end(), [](Point& p1, Point& p2) {return p1.x < p2.x; })->x;
	auto maxY = std::max_element(AIPointsScaled.begin(), AIPointsScaled.end(), [](Point& p1, Point& p2) {return p1.y < p2.y; })->y;
	auto minX = std::min_element(AIPointsScaled.begin(), AIPointsScaled.end(), [](Point& p1, Point& p2) {return p1.x < p2.x; })->x;
	auto minY = std::min_element(AIPointsScaled.begin(), AIPointsScaled.end(), [](Point& p1, Point& p2) {return p1.y < p2.y; })->y;

	auto deltaX = (maxX + minX) / 2;
	auto deltaY = (maxY + minY) / 2;

	std::for_each(AIPointsScaled.begin(), AIPointsScaled.end(), [&](Point& p) { p.x += -deltaX; p.y += -deltaY; });

	auto scaleRatio = std::max(abs(maxX - minX), abs(maxY - minY));

	std::for_each(AIPointsScaled.begin(), AIPointsScaled.end(), [&](Point& p) { p.x *= 5.0 / scaleRatio; p.y *= 5.0 / scaleRatio; });

	gauge = 50.0 / scaleRatio;
	gauge = std::max(gauge, 0.1f);

	createBothRailsForSymmetryAxis(AIPointsScaled, finalLeftRail, finalRightRail, gauge);

	calculateFinalGeometry();
}

void Track::display()
{
	static float highlightCounter = 0;
	highlightCounter += 1.0 / 100.0 * polygons.size();
	if(highlightCounter >= 1.5 * polygons.size())
		highlightCounter -= 2 * polygons.size();


	if (!alreadyPrinted)
	{
		alreadyPrinted = true;

		sin_rz = sin(rz);
		cos_rz = cos(rz);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);


		int id = 0;
		for (auto& polygon : polygons)
		{
			glBindTexture(GL_TEXTURE_2D, polygon.idTexture);

			glBegin(GL_POLYGON);

			if (abs(highlightCounter - id) <= 10)
			{
				glColor3f(1.0f, 0.0f, 0.0f);
			}
			else
			{
				glColor3f(polygon.color.red, polygon.color.green, polygon.color.blue);
			}

			for (unsigned int i = 0; i < polygon.noOfPoints; i++)
			{
				glTexCoord2f(polygon.texturePoints[i].x, polygon.texturePoints[i].y);
				Point toPrint;
				toPrint.x = position.x + polygon.points[i].x * cos_rz - polygon.points[i].y * sin_rz;
				toPrint.y = position.y + polygon.points[i].x * sin_rz + polygon.points[i].y * cos_rz;
				toPrint.z = position.z + polygon.points[i].z;
				glVertex3f(toPrint.x, toPrint.y, toPrint.z);
			}
			glEnd();

			++id;
		}
	}
}

void Track::calculateLength()
{
	length = 0;

	for (int q = 0; q < AIPoints.size(); q++)
	{
		Point& p1 = AIPoints[q];
		Point& p2 = AIPoints[(q + 1) % AIPoints.size()];

		length += p1.distance2D(p2);
	}
}

void Track::updateLapRecordInTxtFile(TrackName trackName, int newRecord, CarBrand carBrand)
{
	auto fileName = "Track_" + (std::to_string(100 + static_cast<int>(trackName))).substr(1, 2) + ".txt";

	std::fstream file;
	file.open("Data/" + fileName);

	std::string line;

	if (file)
	{
		bool recordFound = false;
		char type;

		std::vector<std::string> lines;

		while (getline(file, line))
		{
			std::stringstream ssfulldata(line);
			ssfulldata >> type;

			if (type == 'r')
			{
				line = "r " + std::to_string(newRecord) + " " + std::to_string(static_cast<long>(carBrand)) + " " + currentDateTime();
				recordFound = true;
			}

			lines.push_back(line);
		}

		if(!recordFound)
			lines.push_back("r " + std::to_string(newRecord) + " " + std::to_string(static_cast<long>(carBrand)) + " " + currentDateTime());

		file.clear();
		file.seekg(0, std::ios::beg);
		
		for (auto& updateLine : lines)
		{
			file << updateLine << "\n";
		}
	}

	file.close();
}

TrackRecordData Track::getLapRecordData(TrackName trackName)
{
	TrackRecordData trackRecordData;

	trackRecordData.lapRecord = -1;
	trackRecordData.carBrand = static_cast<CarBrand>(-1);
	trackRecordData.date = "";

	auto fileName = "Track_" + (std::to_string(100 + static_cast<int>(trackName))).substr(1, 2) + ".txt";

	std::ifstream file;
	file.open("Data/" + fileName, std::ios::out);

	if (file)
	{
		char type;
		std::string line;
		while (getline(file, line))
		{
			std::stringstream sline(line);
			sline >> type;

			if (type == 'r')
			{
				int time, carBrand;
				sline >> time;
				trackRecordData.lapRecord = time;

				sline >> carBrand;
				trackRecordData.carBrand = static_cast<CarBrand>(carBrand);

				sline >> trackRecordData.date;
			}
		}
	}

	file.close();

	return trackRecordData;
}