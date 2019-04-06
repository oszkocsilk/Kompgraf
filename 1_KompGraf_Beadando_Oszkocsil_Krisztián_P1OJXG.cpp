#include <GL/glut.h>
#include <bevgrafmath2017.h>
#include <math.h>
#include <vector>
#include<iostream>

using namespace std;

GLsizei winWidth = 800, winHeight = 600;
//vec2 points[256] = { };
vector<vec2> points;
//int count = 0;  //kontrollpontok száma
GLint dragged = -1; // kontrollpontok mozgásához segéd változó

vec4 T;
mat24 G, C;
mat4 M;
vec2 Q;
GLfloat t;


void init()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, winWidth, 0.0f, winHeight, 0.0f, 1.0f);
	glPointSize(4.0);

	M = { -1.0,  3.0, -3.0, 1.0,
			 3.0, -6.0,  0.0, 4.0,
			-3.0,  3.0,  3.0, 1.0,
			 1.0,  0.0,  0.0, 0.0 };

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			M[i][j] = ((M[i][j]) / 6);
}


void SplineDrawer(int n) {
	glLineWidth(2.0);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	vec2 first_P = 2 * points[0] - points[1];
	G = { first_P, points[0], points[1] , points[2] };
	C = G * M;
	for (t = 0; t <= 1; t += 0.01)
	{

		T = { pow(t,3), pow(t,2), t, 1 };
		Q = C * T;
		glVertex2d(Q.x, Q.y);
	}
	glEnd();

	glLineWidth(2.0);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	vec2 last_P = 2 * points[n - 1] - points[n - 2];
	G = { points[n - 3], points[n - 2], points[n - 1], last_P };
	C = G * M;
	for (t = 0; t <= 1; t += 0.01)
	{

		T = { pow(t,3), pow(t,2), t, 1 };
		Q = C * T;
		glVertex2d(Q.x, Q.y);
	}
	glEnd();

	for (int i = 0; i < n - 3; i++)
	{
		glLineWidth(2.0);
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINE_STRIP);
		G = { points[i] , points[i + 1], points[i + 2] , points[i + 3] };
		C = G * M;
		for (t = 0; t <= 1; t += 0.01)
		{

			T = { pow(t,3), pow(t,2), t, 1 };
			Q = C * T;
			glVertex2d(Q.x, Q.y);

		}
		glEnd();

		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
		for (t = 0; t <= 1; t += 1)
		{
			T = { pow(t,3), pow(t,2), t, 1 };
			Q = C * T;
			glVertex2d(Q.x, Q.y);

		}
		glEnd();
	}

}


void display() {

	glClear(GL_COLOR_BUFFER_BIT);
	int n = points.size();


	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	for (int i = 0; i < n; i++) {
		glVertex2f(points[i].x, points[i].y);
	}
	glEnd();

	glLineWidth(1.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	for (int i = 0; i < n - 1; i++) {
		glVertex2f(points[i].x, points[i].y);
		glVertex2f(points[i + 1].x, points[i + 1].y);
	}
	glEnd();


	if (n > 3)
		SplineDrawer(n);


	glFlush();
	glutSwapBuffers();
}

GLint getActivePoint1(GLint x, GLint y) {
	GLint i;
	vec2 P = { (float)x, (float)y };

	for (i = 0; i < points.size(); i++)
		if (dist(points[i], P) < 21)
			return i;
	return -1;
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse) {
	GLint i;
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		if ((i = getActivePoint1(xMouse, winHeight - yMouse)) != -1)
			dragged = i;
		else {
			points.push_back(vec2(xMouse, winHeight - yMouse));
		}
	}
	if (button == GLUT_LEFT_BUTTON && action == GLUT_UP)
		dragged = -1;
	if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN) {
		for (int i = 0; i < points.size(); i++)
		{
			if (dist(points[i], vec2(xMouse, winHeight - yMouse)) < 10)
			{
				points.erase(points.begin() + i);
			}
		}
	}
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse) {
	GLint i;
	if (dragged >= 0) {
		points[dragged].x = xMouse;
		points[dragged].y = winHeight - yMouse;
		glutPostRedisplay();
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(500, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Kompgraf 1. házi");
	init();
	glutDisplayFunc(display);
	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseActiveMotion);
	glutMainLoop();
	return 0;
};