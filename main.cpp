#define _USE_MATH_DEFINES
#include <GL/freeglut.h>
#include <cmath>
#include <iostream>
#include <string>
#include "vec.h"

using namespace std;
int N = 11 + 4;

void Keyboard(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
}

void SpecialKeyboard(int key, int MouseX, int MouseY)
{
	if (key == GLUT_KEY_UP)
		N *= 2;
	if (key == GLUT_KEY_DOWN)
		N /= 2;
	if (key == GLUT_KEY_LEFT)
		N -= 1;
	if (key == GLUT_KEY_RIGHT)
		N += 1;

	if (N <= 0) N = 1;

	glutPostRedisplay();		
}

void PrintCoords(double down_x, double upper_x, double down_y, double upper_y, double delta_x, double delta_y)
{
	glColor3d(0, 0, 0); 
	
	glPointSize(6); 
	glBegin(GL_POINTS);
	for (double i = down_y; i <= upper_y; i += delta_y) glVertex2d(0, i); 
	for (double i = down_x; i <= upper_x; i += delta_x) glVertex2d(i, 0); 
	glEnd();
	
	glLineWidth(2);
	glBegin(GL_LINE_STRIP);
	glVertex2d(down_x, 0);
	glVertex2d(upper_x, 0); 
	glEnd(); 

	glBegin(GL_LINE_STRIP);
	glVertex2d(0, down_y);
	glVertex2d(0, upper_y);
	glEnd();
}

void Sin(double down_x, double upper_x)
{
	glLineWidth(7);
	glColor3d(1, 0, 0);
	glBegin(GL_LINE_STRIP);

	for (double t = down_x; t < upper_x; t += 0.01)
		glVertex2d(t, sin(t));

	glEnd();
}

int minl(int a, int b)
{
	return a < b ? a : b;
}

int maxl(int a, int b)
{
	return a > b ? a : b;
}

void build_catmull(int NumOfr, double down_x, double upper_x)
{
	vec *R = new vec[NumOfr];
	double eps = 0;
	double dx = (upper_x - down_x) / NumOfr, delta = 0;
	for (int i = 0, j = NumOfr - 1; i < NumOfr && j >= 0 && i <= j; i++, j--, delta+=dx)
	{
		if (i == j)
		{
			R[i] = vec(0, sin(0), 0);
			continue;
		}
		
		R[i] = vec(down_x + delta, sin(down_x + delta), 0);
		R[j] = vec(upper_x - delta, sin(upper_x - delta), 0);
	}

	glPointSize(10);
	glColor3d(0, 0, 0);
	glBegin(GL_POINTS);
	for (int i = 0; i < NumOfr; i++)
		glVertex3dv(&R[i].x);
	glEnd();

	for (int i = 0; i < NumOfr - 1; i++)
	{
		vec P0, P1, P2, P3;
		P0 = R[maxl(i-1, 0)];
		P1 = R[i];
		P2 = R[minl(i + 1, NumOfr - 1)];
		P3 = R[minl(i + 2, NumOfr - 1)];
		
		glColor3d(0, 0, 1);
		glLineWidth(3);
		glBegin(GL_LINE_STRIP);

		for (double t = 0; t <= 1 + 0.001; t += 0.01)
		{
			vec Q = (P0 * (-t)  * pow(1 - t, 2) + P1 * (2 - 5 * t * t + 3 * t*t*t) + P2 * (1 + 4 * t - 3 * t*t) * t - P3 * t*t*(1 - t))*0.5;
			glVertex3f(Q.x, Q.y, Q.z);
			eps += abs(Q.y - sin(Q.x));
		}
		glEnd();
	}
	printf("For %i amout of points Delta is: %0.10lf.(catmull)\n", NumOfr, eps/(100*NumOfr));
	
}

void build_bezier(int NumOfr, double down_x, double upper_x)
{
	vec *R = new vec[NumOfr];
	double eps = 0;
	double dx = (upper_x - down_x) / NumOfr, delta = 0;
	for (int i = 0, j = NumOfr - 1; i < NumOfr && j >= 0 && i <= j; i++, j--, delta += dx)
	{
		if (i == j)
		{
			R[i] = vec(0, sin(0), 0);
			continue;
		}

		R[i] = vec(down_x + delta, sin(down_x + delta), 0);
		R[j] = vec(upper_x - delta, sin(upper_x - delta), 0);
	}

	glPointSize(10);
	glColor3d(0, 0, 0);
	glBegin(GL_POINTS);
	for (int i = 0; i < NumOfr; i++)
		glVertex3dv(&R[i].x);
	glEnd();

	for (int i = 1; i < NumOfr-1; i+=2)
	{
		vec P0, P1, P2, P3;
		if (i == 1) P0 = R[0];
		else P0 = R[i] | R[i - 1];

		P1 = R[i];
		P2 = R[i+1];

		if (i + 2 == NumOfr - 1) P3 = R[i + 2];
		else if (i + 2 > NumOfr - 1) P3 = R[i + 1];
		else P3 = R[i + 1] | R[i + 2];

		glColor3d(0, 0, 1);
		glLineWidth(3);
		glBegin(GL_LINE_STRIP);

		for (double t = 0; t <= 1 + 0.001; t += 0.01)
		{
			vec Q = P0 * pow(1 - t, 3) + P1 * 3 * pow(1 - t, 2) * t + P2 * 3 * t * t * (1 - t) + P3 * t*t*t;
			glVertex3f(Q.x, Q.y, Q.z);
			eps += abs(Q.y - sin(Q.x));
		}
		glEnd();
	}
	printf("For %i amout of points Delta is: %0.10lf(bezier).\n", NumOfr, eps / (100 * NumOfr));
}

void Init(void)
{
	glClearColor(1, 1, 1, 0);
}

void Display_CatMull(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glPushMatrix();
	glLineWidth(1);
	glScalef(0.19, 0.30, 0);

	PrintCoords(-5, 5, -2, 2, 1, 1);
	Sin(-1.5*M_PI, 1.5*M_PI);
	build_catmull(N, -M_PI, M_PI);

	glLineWidth(1);
	glPopMatrix();

	glutSwapBuffers();
}

void Display_Bezier(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	glLineWidth(1);
	glScalef(0.19, 0.30, 0);

	PrintCoords(-5, 5, -2, 2, 1, 1);
	Sin(-1.5*M_PI, 1.5*M_PI);
	build_bezier(N, -M_PI, M_PI);

	glLineWidth(1);
	glPopMatrix();
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(30, 30);
	glutInitWindowSize(1000, 500);

	glutCreateWindow("CatMull-Rom");
	glutDisplayFunc(Display_CatMull);
	Init();
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);

	glutCreateWindow("Bezier");
	glutDisplayFunc(Display_Bezier);
	Init();
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);

	glutMainLoop();
}