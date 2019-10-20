#include "Object3D.h"
#include <fstream>
#include <sstream>

Object3D::Object3D()
{


}

void Object3D::loadModel()
{
	
}

void Object3D::display()
{
	if (!alreadyPrinted)
	{
		alreadyPrinted = true;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);

		for (auto& polygon : polygons)
		{
			glBindTexture(GL_TEXTURE_2D, polygon.idTexture);

			glBegin(GL_POLYGON);
			glColor3f(polygon.color.red, polygon.color.green, polygon.color.blue);

			for (unsigned int i = 0; i < polygon.noOfPoints; i++)
			{
				glTexCoord2f(polygon.texturePoints[i].x, polygon.texturePoints[i].y);
				glVertex3f(polygon.points[i].x, polygon.points[i].y, polygon.points[i].z);
			}
			glEnd();
		}

		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
	}
}

std::map<std::string, Color> Object3D::importMaterials(const char* filePath)
{
	std::map<std::string, Color> materials;

	std::ifstream file;
	file.open(filePath);

	if (!file)
	{
		std::cout << "ERROR: missing file: " << filePath << "\a";
		Sleep(2000);
		throw;
	}

	std::string line;
	std::string svalue;
	std::string materalName;

	do
	{
		getline(file, line);
		std::stringstream ssline(line);

		ssline >> svalue;

		if (svalue == "newmtl")
		{
			ssline >> materalName;

			float r = 1, g = 1, b = 1;

			getline(file, line);
			std::stringstream ssfulldata(line);
			getline(ssfulldata, svalue, ' ');

			getline(ssfulldata, svalue, ' ');
			r = stof(svalue);

			getline(ssfulldata, svalue, ' ');
			g = stof(svalue);

			getline(ssfulldata, svalue);
			b = stof(svalue);

			materials.insert({ materalName, Color(r, g, b)});

		}
	} while (line != "#EOF" && line != "# End of File");

	file.close();

	return materials;
}

void Object3D::importFromObjFile(const char* filePath, Textures textureName, float scaleRatio, std::map<std::string, Color>* materials)
{
	std::ifstream file;
	file.open(filePath);

	if (!file)
	{
		std::cout << "ERROR: missing file: " << filePath << "\a";
		Sleep(2000);
		throw;
	}

	std::vector<Point> vertices(1);
	std::vector<Point> textureVertices(1);
	std::vector<std::vector<int>> faces;


	std::string line;
	std::string svalue;

	Color currentColor = Color(ColorName::WHITE);
	bool texture = true;

	do
	{
		getline(file, line);
		std::stringstream ssline(line);

		ssline >> svalue;

		if (svalue == "v")
		{
			Point newPoint;
			ssline >> newPoint.x;
			ssline >> newPoint.z;
			ssline >> newPoint.y;

			//newPoint.x /= 10;
			//newPoint.y /= 10;
			//newPoint.z /= 10;

			vertices.push_back(newPoint);
		}
		else if (svalue == "vt")
		{
			Point newPoint;
			ssline >> newPoint.x;
			ssline >> newPoint.y;

			textureVertices.push_back(newPoint);
		}
		else if (svalue == "f")
		{
			int v = 0, vt = 0, vn = 0;
			Polygon newPolygon;

			std::stringstream ssfulldata(line);
			getline(ssfulldata, svalue, ' ');

			while (getline(ssfulldata, svalue, ' '))
			{
				std::stringstream data(svalue);
				getline(data, svalue, '/');
				v = stoi(svalue);

				getline(data, svalue, '/');
				vt = stoi(svalue);

				getline(data, svalue, '/');
				vn = stoi(svalue);

				newPolygon.points.push_back(vertices[v]);
				newPolygon.texturePoints.push_back(textureVertices[vt]);
			}

			newPolygon.noOfPoints = newPolygon.points.size();
			newPolygon.color = currentColor;
			newPolygon.additionalColor = Color(ColorName::WHITE);

			if(texture)
				newPolygon.idTexture = TextureManager::Instance()->textures[static_cast<int>(textureName)].idTexture;
			else
				newPolygon.idTexture = TextureManager::Instance()->textures[static_cast<int>(Textures::no_texture)].idTexture;

			polygons.push_back(newPolygon);

		}
		else if (svalue == "usemtl")
		{
			if (materials)
			{
				std::string materialName;
				ssline >> materialName;

				auto it = materials->find(materialName);
				if (it != materials->end() && materialName.find("Color") != std::string::npos)
				{
					currentColor = it->second;
					texture = false;
				}
				else
				{
					currentColor = Color(ColorName::WHITE);
					texture = true;
				}
			}
			else
			{
				currentColor = Color(ColorName::WHITE);
				texture = true;
			}

			//vertices.resize(1);
			//textureVertices.resize(1);
		}
	} while (line != "#EOF" && line != "# End of File");

	float maxX = -100.0f;
	float maxY = -100.0f;
	float minX =  100.0f;
	float minY =  100.0f;

	for (auto& polygon : polygons)
	{
		for (auto& point : polygon.points)
		{
			if (point.x > maxX)
			{
				maxX = point.x;
			}
			if (point.y > maxY)
			{
				maxY = point.y;
			}
			if (point.x < minX)
			{
				minX = point.x;
			}
			if (point.y < minY)
			{
				minY = point.y;
			}
		}

	}

	auto deltaX = maxX - minX;
	auto deltaY = maxY - minY;

	for (auto& polygon : polygons)
	{
		for (auto& point : polygon.points)
		{
			point.x = point.x - minX - 0.5 * deltaX;
			point.y = point.y - minY - 0.5 * deltaY;

			point.x *= -scaleRatio;
			point.y *= scaleRatio;
			point.z *= scaleRatio;
		}
	}

	file.close();
}

void Object3D::cretateGlobalVertex(const Point& localPoint)
{
	Point toPrint;
	toPrint.x = position.x + localPoint.x * cos_rz - localPoint.y * sin_rz;
	toPrint.y = position.y + localPoint.x * sin_rz + localPoint.y * cos_rz;
	toPrint.z = position.z + localPoint.z;
	glVertex3f(toPrint.x, toPrint.y, toPrint.z);
}

vector2D Object3D::getGlobalVector(const vector2D& vLocal)
{
	return { vLocal.x * cos(rz) - vLocal.y * sin(rz), vLocal.x * sin(rz) + vLocal.y * cos(rz) };
}

vector2D Object3D::getLocalVector(const vector2D& vGlobal)
{
	return { vGlobal.x * cos(-rz) - vGlobal.y * sin(-rz), vGlobal.x * sin(-rz) + vGlobal.y * cos(-rz) };
}

void Object3D::myglDrawCircle(const Point& center, float r, int segments)
{
	glBegin(GL_LINE_LOOP);
	for (float alpha = 0.0f; alpha < 2 * PI; alpha += 2 * PI / segments)
	{
		glVertex3f(center.x + r * cos(alpha), center.y + r * sin(alpha), center.z);
	}
	glEnd();
}

void Object3D::myglDrawFillCircle(const Point& center, float r, int segments)
{
	glBegin(GL_POLYGON);
	for (float alpha = 0.0f; alpha < 2 * PI; alpha += 2 * PI / segments)
	{
		glVertex3f(center.x + r * cos(alpha), center.y + r * sin(alpha), center.z);
	}
	glEnd();
}