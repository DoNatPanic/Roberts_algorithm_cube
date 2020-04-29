#include <windows.h>
#include "glut.h"
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <map>
#include "algCG.h"

using namespace std;
using namespace algCG;

#define GL_GLEXT_PROTOTYPES
float R = 25.0;
view_point P = { 0.0, 0.0, -R };

struct Color {
	float r;
	float g;
	float b;
};

double rotate_y = 0;  // = phi
double rotate_x = 0;  // = theta

//modelview transformation - not used
/*vertex& transformation(vertex& initial)
{
	vertex result;
	result.X = cos(rotate_y) * initial.X - sin(rotate_y) * sin(rotate_x) * initial.Y - sin(rotate_y) * cos(rotate_x) * initial.Z;
	result.Y = cos(rotate_x) * initial.Y - sin(rotate_x) * initial.Z;
	result.Z = sin(rotate_y) * initial.X + cos(rotate_y) * sin(rotate_x) * initial.Y + cos(rotate_y) * cos(rotate_x) * initial.Z;
	return result;
}*/


//points coordinates
vertex p0 = { -0.5, -0.5, -0.5 };
vertex p1 = { 0.5, -0.5, -0.5 };
vertex p2 = { 0.5, 0.5, -0.5 };
vertex p3 = { -0.5, 0.5, -0.5 };
vertex p4 = { -0.5, -0.5, 0.5 };
vertex p5 = { 0.5, -0.5, 0.5 };
vertex p6 = { 0.5, 0.5, 0.5 };
vertex p7 = { -0.5, 0.5, 0.5 };

void DrawLine(Color color, vertex p1, vertex p2) {
	glBegin(GL_LINES);
	glColor3f(color.r, color.g, color.b);
	glVertex3f(p1.X, p1.Y, p1.Z);
	glVertex3f(p2.X, p2.Y, p2.Z);
	glEnd();
}

void display() {

	//  Clear screen and Z-buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

	// Rotate when user changes rotate_x and rotate_y
	//glTranslatef(0, 0, 0);
	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 1.0, 0.0);

	vertex V[8];
	V[0] = p0; V[1] = p1; V[2] = p2; V[3] = p3;//точки 0-7
	V[4] = p4; V[5] = p5; V[6] = p6; V[7] = p7;

	// barycenter determination
	//
	barycenter W;
	for (int counter = 0; counter < 8; ++counter)
	{
		W.X += V[counter].X;
		W.Y += V[counter].Y;
		W.Z += V[counter].Z;
	}
	W.X /= 8;
	W.Y /= 8;
	W.Z /= 8;
	//Draw all lines - not used
	/*DrawLine({ 0.0, 1.0, 0.0 }, p0, p1);//0
	DrawLine({ 1.0, 1.0, 1.0 }, p1, p2);//1
	DrawLine({ 1.0, 1.0, 1.0 }, p3, p2);//2
	DrawLine({ 1.0, 1.0, 1.0 }, p0, p3);//3
	DrawLine({ 1.0, 1.0, 1.0 }, p4, p5);//4
	DrawLine({ 1.0, 1.0, 1.0 }, p5, p6);//5
	DrawLine({ 1.0, 1.0, 1.0 }, p7, p6);//6
	DrawLine({ 1.0, 1.0, 1.0 }, p4, p7);//7
	DrawLine({ 1.0, 1.0, 1.0 }, p3, p7);//8
	DrawLine({ 1.0, 1.0, 1.0 }, p0, p4);//9
	DrawLine({ 1.0, 1.0, 1.0 }, p2, p6);//10
	DrawLine({ 1.0, 1.0, 0.0 }, p1, p5);//11*/
	
	if (!Roberts_alg(V[0], V[1], V[2], P, W)) {
		DrawLine({ 1.0, 1.0, 1.0 }, p0, p1);//0
		DrawLine({ 1.0, 1.0, 1.0 }, p1, p2);//1
		DrawLine({ 1.0, 1.0, 1.0 }, p3, p2);//2
		DrawLine({ 1.0, 1.0, 1.0 }, p0, p3);//3
	}
	if (!Roberts_alg(V[1], V[5], V[6], P, W)) {
		DrawLine({ 1.0, 1.0, 1.0 }, p1, p2);//1
		DrawLine({ 1.0, 1.0, 1.0 }, p5, p6);//5
		DrawLine({ 1.0, 1.0, 1.0 }, p2, p6);//10
		DrawLine({ 1.0, 1.0, 1.0 }, p1, p5);//11
	}
	if (!Roberts_alg(V[5], V[4], V[7], P, W)) {
		DrawLine({ 1.0, 1.0, 1.0 }, p4, p5);//4
		DrawLine({ 1.0, 1.0, 1.0 }, p5, p6);//5
		DrawLine({ 1.0, 1.0, 1.0 }, p7, p6);//6
		DrawLine({ 1.0, 1.0, 1.0 }, p4, p7);//7
	}
	if (!Roberts_alg(V[4], V[0], V[3], P, W)) {
		DrawLine({ 1.0, 1.0, 1.0 }, p0, p3);//3
		DrawLine({ 1.0, 1.0, 1.0 }, p4, p7);//7
		DrawLine({ 1.0, 1.0, 1.0 }, p3, p7);//8
		DrawLine({ 1.0, 1.0, 1.0 }, p0, p4);//9
	}
	if (!Roberts_alg(V[3], V[2], V[6], P, W)) {
		DrawLine({ 1.0, 1.0, 1.0 }, p3, p2);//2
		DrawLine({ 1.0, 1.0, 1.0 }, p7, p6);//6
		DrawLine({ 1.0, 1.0, 1.0 }, p3, p7);//8
		DrawLine({ 1.0, 1.0, 1.0 }, p2, p6);//10
	}
	if (!Roberts_alg(V[0], V[1], V[5], P, W)) {
		DrawLine({ 1.0, 1.0, 1.0 }, p0, p1);//0
		DrawLine({ 1.0, 1.0, 1.0 }, p4, p5);//4
		DrawLine({ 1.0, 1.0, 1.0 }, p0, p4);//9
		DrawLine({ 1.0, 1.0, 1.0 }, p1, p5);//11
	}

	glFlush();
	glutSwapBuffers();
}
void specialKeys(int key, int x, int y) {
	
	if (key == GLUT_KEY_RIGHT) {
			rotate_y += 5.0;
	}

	else if (key == GLUT_KEY_LEFT) {
			rotate_y -= 5.0;
	}

	else if (key == GLUT_KEY_UP) {
			rotate_x -= 5.0;
	}
		
	else if (key == GLUT_KEY_DOWN) {
			rotate_x += 5.0;
	}

	float phi = abs(rotate_y) * 3.14 / 180.0;
	float theta = rotate_x * 3.14 / 180.0;

	float theta_90 = abs(rotate_x - 90) * 3.14 / 180.0;

	int k_x = 0;
	if (rotate_x >= 0 && rotate_x <= 90)
		k_x = -1;
	else
		k_x = 1;
	
	P.X = R * cos(theta) * sin(phi);
	P.Z = - R * cos(theta) * cos(phi);

	if (abs(rotate_x) <= 90)
		P.Y = 2 * R * k_x* sin(theta / 2.0) * sin(theta / 2.0);
	else 
		P.Y = - R + 2 * R * k_x * sin(theta_90 / 2.0) * sin(theta_90 / 2.0);


	printf("%f %f %f %f %f\n", rotate_x, rotate_y, P.X, P.Y, P.Z);

	//  Request display update
	glutPostRedisplay();
}

int main(int argc, char* argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Cube");
	glEnable(GL_DEPTH_TEST);
	glutDisplayFunc(display);
	glutSpecialFunc(specialKeys);
	glutMainLoop();

	return 0;
}
