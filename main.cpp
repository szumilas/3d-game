#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <vector>

struct Point
{
	float X;
	float Y;
	float Z;
};

class Camera
{
public:
	Camera() { x = 15; y = -30; z = 10;	rx = 0;	ry = 0; rz = 0; }

	void moveX(float dx) { x += dx; }
	void moveY(float dy) { y += dy; }
	void moveZ(float dz) { z += dz; }

	void rotateX(float drx) { rx += drx; }
	void rotateY(float dry) { ry += dry; }
	void rotateZ(float drz) { rz += drz; }

public:
	float x;
	float y;
	float z;

	float rx;
	float ry;
	float rz;

};

class Car
{
public:

	Car()
	{
		X = 15;
		Y = -15;
		Z = 0.01;
		rz = 0;
		a = 0;
		v = 0;

		/*points.push_back({ 2.0f, 0.7f, 0.0f });
		points.push_back({ 2.0f, -0.7f, 0.0f });
		points.push_back({ -2.0f, -0.7f, 0.0f });
		points.push_back({ -2.0f, 0.7f, 0.0f });*/

		points_left.push_back({ 2.0615f, 0.8175f, 0.281f });
		points_left.push_back({ 1.6095f, 0.8175f, 0.241f });
		points_left.push_back({ 1.5375f, 0.8175f, 0.509f });
		points_left.push_back({ 1.2735f, 0.8175f, 0.625f });
		points_left.push_back({ 1.0375f, 0.8175f, 0.513f });
		points_left.push_back({ 0.9615f, 0.8175f, 0.257f });
		points_left.push_back({ -0.2585f, 0.8175f, 0.277f });
		points_left.push_back({ -0.8785f, 0.8175f, 0.229f });
		points_left.push_back({ -0.9425f, 0.8175f, 0.477f });
		points_left.push_back({ -1.2105f, 0.8175f, 0.617f });
		points_left.push_back({ -1.4385f, 0.8175f, 0.525f });
		points_left.push_back({ -1.5385f, 0.8175f, 0.313f });
		points_left.push_back({ -2.0305f, 0.8175f, 0.333f });
		points_left.push_back({ -2.1065f, 0.8175f, 0.545f });
		points_left.push_back({ -2.0265f, 0.8175f, 0.629f });
		points_left.push_back({ -2.0025f, 0.8175f, 0.941f });
		points_left.push_back({ -1.2545f, 0.8175f, 1.369f });
		points_left.push_back({ 0.2975f, 0.8175f, 1.369f });
		points_left.push_back({ 1.0455f, 0.8175f, 0.937f });
		points_left.push_back({ 1.9895f, 0.8175f, 0.745f });
		points_left.push_back({ 2.1065f, 0.8175f, 0.505f });

		points_right.push_back({ 2.0615f,  -0.8175f, 0.281f });
		points_right.push_back({ 1.6095f,  -0.8175f, 0.241f });
		points_right.push_back({ 1.5375f,  -0.8175f, 0.509f });
		points_right.push_back({ 1.2735f,  -0.8175f, 0.625f });
		points_right.push_back({ 1.0375f,  -0.8175f, 0.513f });
		points_right.push_back({ 0.9615f,  -0.8175f, 0.257f });
		points_right.push_back({ -0.2585f, -0.8175f, 0.277f });
		points_right.push_back({ -0.8785f, -0.8175f, 0.229f });
		points_right.push_back({ -0.9425f, -0.8175f, 0.477f });
		points_right.push_back({ -1.2105f, -0.8175f, 0.617f });
		points_right.push_back({ -1.4385f, -0.8175f, 0.525f });
		points_right.push_back({ -1.5385f, -0.8175f, 0.313f });
		points_right.push_back({ -2.0305f, -0.8175f, 0.333f });
		points_right.push_back({ -2.1065f, -0.8175f, 0.545f });
		points_right.push_back({ -2.0265f, -0.8175f, 0.629f });
		points_right.push_back({ -2.0025f, -0.8175f, 0.941f });
		points_right.push_back({ -1.2545f, -0.8175f, 1.369f });
		points_right.push_back({ 0.2975f,  -0.8175f, 1.369f });
		points_right.push_back({ 1.0455f,  -0.8175f, 0.937f });
		points_right.push_back({ 1.9895f,  -0.8175f, 0.745f });
		points_right.push_back({ 2.1065f,  -0.8175f, 0.505f });

	
	};

	void print()
	{
		
		
		glBegin(GL_POLYGON);


		glColor3f(0.1, 0.7, 0.1);


		for (auto& point : points_left)
		{
			glVertex3f(X + point.X * cos_rz - point.Y * sin_rz, Y + point.X * sin_rz + point.Y * cos_rz, Z + point.Z);
		}

		glEnd();

		glBegin(GL_POLYGON);


		glColor3f(0.1, 0.7, 0.1);


		for (auto& point : points_right)
		{
			glVertex3f(X + point.X * cos_rz - point.Y * sin_rz, Y + point.X * sin_rz + point.Y * cos_rz, Z + point.Z);
		}

		glEnd();

		glColor3f(0.1, 0.8, 0.1);

		glBegin(GL_QUADS);

		for (size_t q = 0; q < points_left.size() - 1; q++)
		{
			auto point_left = points_left[q];
			auto point_right = points_right[q];
			auto point_left_next = points_left[q + 1];
			auto point_right_next = points_right[q + 1];

			glVertex3f(X + point_left.X * cos_rz - point_left.Y * sin_rz, Y + point_left.X * sin_rz + point_left.Y * cos_rz, Z + point_left.Z);
			glVertex3f(X + point_left_next.X * cos_rz - point_left_next.Y * sin_rz, Y + point_left_next.X * sin_rz + point_left_next.Y * cos_rz, Z + point_left_next.Z);
			glVertex3f(X + point_right_next.X * cos_rz - point_right_next.Y * sin_rz, Y + point_right_next.X * sin_rz + point_right_next.Y * cos_rz, Z + point_right_next.Z);
			glVertex3f(X + point_right.X * cos_rz - point_right.Y * sin_rz, Y + point_right.X * sin_rz + point_right.Y * cos_rz, Z + point_right.Z);


		}

		glEnd();


		/*
		glVertex3f(X + 0.045, Y, Z + 0.281);
		glVertex3f(X + 0.497, Y, Z + 0.241);
		glVertex3f(X + 0.569, Y, Z + 0.509);
		glVertex3f(X + 0.833, Y, Z + 0.625);
		glVertex3f(X + 1.069, Y, Z + 0.513);
		glVertex3f(X + 1.145, Y, Z + 0.257);
		glVertex3f(X + 2.365, Y, Z + 0.277);
		glVertex3f(X + 2.985, Y, Z + 0.229);
		glVertex3f(X + 3.049, Y, Z + 0.477);
		glVertex3f(X + 3.317, Y, Z + 0.617);
		glVertex3f(X + 3.545, Y, Z + 0.525);
		glVertex3f(X + 3.645, Y, Z + 0.313);
		glVertex3f(X + 4.137, Y, Z + 0.333);
		glVertex3f(X + 4.213, Y, Z + 0.545);
		glVertex3f(X + 4.133, Y, Z + 0.629);
		glVertex3f(X + 4.109, Y, Z + 0.941);
		glVertex3f(X + 3.361, Y, Z + 1.369);
		glVertex3f(X + 1.809, Y, Z + 1.369);
		glVertex3f(X + 1.061, Y, Z + 0.937);
		glVertex3f(X + 0.117, Y, Z + 0.745);
		glVertex3f(X + 0, Y, Z + 0.505);
		*/



		/*
		glBegin(GL_TRIANGLES);

		glColor3f(0.5, 0.1, 0.1);
		glVertex3f(X + cos(rz) * L, Y - sin(rz) * L, Z + 0);
		glVertex3f(X - sin(rz) * B, Y - cos(rz) * B, Z);
		glVertex3f(X, Y, Z + 2);

		glColor3f(0.5, 0.5, 0.1);
		glVertex3f(X + cos(rz) * L, Y - sin(rz) * L, Z + 0);
		glVertex3f(X, Y, Z + 2);
		glVertex3f(X + sin(rz) * B, Y + cos(rz) * B, Z);


		glColor3f(0.8, 0.1, 0.1);
		glVertex3f(X + cos(rz) * L, Y - sin(rz) * L, Z);
		glVertex3f(X - sin(rz) * B, Y - cos(rz) * B, Z);
		glVertex3f(X + sin(rz) * B, Y + cos(rz) * B, Z);


		glColor4f(0.3, 0.1, 0.3, 0.5);
		glVertex3f(X - sin(rz) * B, Y - cos(rz) * B, Z);
		glVertex3f(X, Y, Z + 2);
		glVertex3f(X + sin(rz) * B, Y + cos(rz) * B, Z);

		glEnd();
		
		glColor3f(0, 0, 0);
		glBegin(GL_LINES);

		glVertex3f(X + cos(rz) * L, Y - sin(rz) * L, Z + 0);
		glVertex3f(X, Y, Z + 2);

		glVertex3f(X, Y, Z + 2);
		glVertex3f(X + sin(rz) * B, Y + cos(rz) * B, Z);

		glVertex3f(X - sin(rz) * B, Y - cos(rz) * B, Z);
		glVertex3f(X, Y, Z + 2);

		glEnd();
		*/

	}
	void move()
	{
		sin_rz = sin(rz);
		cos_rz = cos(rz);

		X += v * cos(rz) * 0.1;
		Y += v * sin(rz) * 0.1;
	}

	void accelerate()
	{
		v += 0.001;
		if (v > v_max)
			v = v_max;
	}

	void slow()
	{
		v -= 0.001;
		if (v < -v_max)
			v = -v_max;
	}

	void turnRight()
	{
		rz -= 0.005;
	}

	void turnLeft()
	{
		rz += 0.005;
	}

	float X;
	float Y;
	float Z;
	float rz;

	float sin_rz;
	float cos_rz;

	float a;
	float v;

	float v_max = 0.15;
	float L = 5;
	float B = 1;

	std::vector<Point> points_left;
	std::vector<Point> points_right;


};

void display();
void reshape(int, int);
void keyboard(unsigned char key, int x, int y);

void SpecialKeys(int key, int x, int y);
void SpecialKeysUp(int key, int x, int y);
void Update();

void Cube(float a, float b, float c, float d, float h);

Camera camera;
Car car;

GLboolean upPressed = false;
GLboolean downPressed = false;
GLboolean leftPressed = false;
GLboolean rightPressed = false;

void init()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char**agrv)
{
	glutInit(&argc, agrv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowPosition(10, 10);
	glutInitWindowSize(1500, 750);

	glutCreateWindow("3d game");	

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutKeyboardFunc(keyboard);
	glutSpecialFunc(SpecialKeys);
	glutSpecialUpFunc(SpecialKeysUp);
	glutIdleFunc(Update);

	init();


	glutMainLoop();

	return 0;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();

	

	//glTranslatef(camera.x, camera.y, camera.z);
	//glRotated(camera.rx, camera.x, 1.0, camera.z);

	gluLookAt(camera.x, camera.y, camera.z, //eye
		camera.x, camera.y + 1, camera.z - 1, //center
		0, 0, 1); //up

	for (int X = -30; X < 30; X++)
	{
		for (int Y = -30; Y < 30; Y++)
		{
			if ((X + Y) % 2 == 0)
			{

				glBegin(GL_POLYGON);
				glColor3f(1, 1, 1);
				glVertex3f(X + 0.0, Y + 0.0, 0.0);
				glVertex3f(X + 1.0, Y + 0.0, 0.0);
				glVertex3f(X + 1.0, Y + 1.0, 0.0);
				glVertex3f(X + 0.0, Y + 1.0, 0.0);

				glEnd();
			}


		}
	}

	Cube(5, 5, 6, 6, 3);
	Cube(15, 8, 18, 12, 8);
	Cube(-25, -15, -16, -6, 4);
	Cube(0, 0, 1, 1, 8);
	Cube(-5, 5, -6, 6, 4);

	car.print();

	glBegin(GL_LINES);

	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 50.0, 0.0);

	glEnd();

	glBegin(GL_LINES);

	glVertex3f(15.0, 0.0, 0.0);
	glVertex3f(15.0, 50.0, 0.0);

	glEnd();



	glutSwapBuffers();

}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluPerspective(45, width / height, 0.5, 150);
	glMatrixMode(GL_MODELVIEW);

	display();
}


void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: // Escape key
		
		exit(0);
		break;
	}


	std::vector<bool> keys(100);
	for (int q = 0; q < keys.size(); q++)
		keys[q] = GetAsyncKeyState(q);



	if (keys[65])
		camera.rotateY(-0.02);

	if (keys[87])
		camera.rotateX(-0.02);

	if (keys[68])
		camera.rotateY(0.02);

	if (keys[83])
		camera.rotateX(0.02);

	//translate
	if (keys[70]) //F
		camera.moveX(0.1);

	if (keys[72]) //H
		camera.moveX(-0.1);

	if (keys[84]) //T
		camera.moveZ(0.1);

	if (keys[71]) //G
		camera.moveZ(-0.1);

	if (keys[82]) //R
		camera.moveY(0.1);

	if (keys[89]) //Y
		camera.moveY(-0.1);

	reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

void SpecialKeys(int key, int x, int y)
{

	if (GLUT_KEY_UP == key)
		upPressed = true;
	if (GLUT_KEY_DOWN == key)
		downPressed = true;
	if (GLUT_KEY_LEFT == key)
		leftPressed = true;
	if (GLUT_KEY_RIGHT == key)
		rightPressed = true;


}

void SpecialKeysUp(int key, int x, int y)
{
	if (GLUT_KEY_UP == key)
		upPressed = false;
	if (GLUT_KEY_DOWN == key)
		downPressed = false;
	if (GLUT_KEY_LEFT == key)
		leftPressed = false;
	if (GLUT_KEY_RIGHT == key)
		rightPressed = false;

}

void Update()
{
	/*
	if (upPressed)
		camera.moveZ(0.01);
	if (downPressed)
		camera.moveZ(-0.01);
	if (leftPressed)
		camera.moveX(0.01);
	if (rightPressed)
		camera.moveX(-0.01);
	*/
	if (upPressed)
		car.accelerate();
	if (downPressed)
		car.slow();
	if (leftPressed)
		car.turnLeft();
	if (rightPressed)
		car.turnRight();

	car.move();

	reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

void Cube(float a, float b, float c, float d, float h)
{
	glColor3f(0, 0.5, 0);

	glBegin(GL_POLYGON);

	glVertex3f(c, 0.0, d);
	glVertex3f(a, 0.0, d);
	glVertex3f(a, h, d);
	glVertex3f(c, h, d);

	glEnd();

	glColor3f(0.5, 0, 0);

	glBegin(GL_QUADS);

	glVertex3f(a, 0.0, b);
	glVertex3f(a, 0.0, d);
	glVertex3f(a, h, d);
	glVertex3f(a, h, b);

	glEnd();

	glColor3f(0.5, 0, 0.5);

	glBegin(GL_QUADS);

	glVertex3f(c, 0.0, b);
	glVertex3f(c, 0.0, d);
	glVertex3f(c, h, d);
	glVertex3f(c, h, b);

	glEnd();

	glColor3f(0.5, 0.5, 0);

	glBegin(GL_QUADS);

	glVertex3f(a, 0.0, b);
	glVertex3f(c, 0.0, b);
	glVertex3f(c, h, b);
	glVertex3f(a, h, b);

	glEnd();


}

