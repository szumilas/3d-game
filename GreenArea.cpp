#include "GreenArea.h"

void GreenArea::calculateFinalGeometry(TextureManager* textureManager)
{
	idTexture = textureManager->textures[static_cast<long>(Textures::grass)].idTexture;
}

void GreenArea::display()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, idTexture);
	glEnable(GL_TEXTURE_2D);

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);

	for (auto& point : points)
	{
		glTexCoord2f(point.x, point.y);
		glVertex3f(point.x, point.y, 0.01f);
	}

	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);




}