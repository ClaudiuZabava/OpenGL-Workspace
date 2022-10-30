#include <windows.h>  // biblioteci care urmeaza sa fie incluse
#include <stdlib.h> // necesare pentru citirea shader-elor
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GL/glew.h> // glew apare inainte de freeglut
#include <GL/freeglut.h> // nu trebuie uitat freeglut.h
#include "loadShaders.h"

// Din biblioteca glm
#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;

GLuint
VaoId,
VboId,
ColorBufferId,
ProgramId,
myMatrixLocation,
matrScaleLocation,
matrTranslLocation,
matrRotlLocation,
codColLocation;



/*

		car1 = masina de jos care e racheta aka depaseste => Bentley Bentayga

		car2 = masina de jos care urmeaza a fi depasita

		car3 = masina de sus care circula normal

*/



/*

	TO DO:
		1. Coliziuni. 
		2. Style. { Girofaruri la racheta, flashuri: la racheta mari si la restul mici, stopuri la toate 3}


*/



int colorCode;

float PI = 3.141592, angle = 0, angle2 = 0;

float tx = 0; float ty = 0;

float width = 450, height = 300;

float i1 = -450.0, i2 = -200.0, i3 = 100.0, k1 = 0.0;

float alpha1 = 0.0, alpha2 = 0.0, alpha3 = 0.0, alpha4 = 0.0, beta = 0.05;

float speed2 = 0.3, speed3 = 0.5, speed1 = 1.5;

glm::mat4

myMatrix, resizeMatrix, matrTransl1, matrTranslAsphalt, matrTransl3, matrTransl2, matrScaleCars, matrScale2, matrScaleAsphalt, matrScaleBackground, matrRot, matrDepl , matrScaleLights, matrTransl4, matrScaleBeacon1, matrScaleBeacon2, matrRotBeacon1, matrRotBeacon2, matrTransl5, matrTransl6, matrTransl7, matrScaleStops, matrTransl8, matrTransl9, matrTransl10;




void showMatrix()
{
	for (int ii = 0; ii < 4; ii++)
	{
		for (int jj = 0; jj < 4; jj++)
			cout << myMatrix[ii][jj] << "  ";
		cout << endl;
	};
	cout << "\n";
};



void movement(void)
{

	i1 = i1 + alpha1;
	if (i1 > 500)
		alpha1 = 0.0;
	angle = angle - beta;

	if (i2 <= i1 + 80)
	{
		k1 = k1 + alpha4;
		if (k1 > 100.0)
			alpha4 = -speed1;
		else if (k1 < 0.0)
			alpha4 = 0.0;
		angle = angle - beta;
	}



	i2 = i2 + alpha2;
	if (i2 > 500.0)
		alpha2 = 0.0;
	else if (i2 < -350.0)
		alpha2 = speed2;
	angle = angle - beta;



	i3 = i3 + alpha3;
	if (i3 > 350.0)
		alpha3 = -speed3;
	else if (i3 < -550.0)
		alpha3 = 0.0;
	angle2 = angle2 + beta;

	if (k1 >= 100 && ((i1+25 >= i3 - 25 && i1 <= i3) || (i3+25 >=i1-25 && i3<=i1)))
	{
		alpha1 = 0.0;
		alpha3 = 0.0;
		alpha4 = 0.0;

	}



	glutPostRedisplay();

}



void keyPress(int button, int state, int x, int y)
{
	switch (button) {

	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			alpha1 = speed2;
			alpha2 = speed2;
			alpha3 = -speed3;
		}
		glutIdleFunc(movement);
		break;


	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
			speed1 = speed2*5;
			alpha1 = speed1;
			alpha4 = speed1;
		}

		glutIdleFunc(movement);

		break;


	default:
		break;
	}
}



void CreateVBO(void)
{
	// varfurile 
	GLfloat Vertices[] = {
		// varfuri pentru axe
		-450.0f, 0.0f, 0.0f, 1.0f,
		450.0f,  0.0f, 0.0f, 1.0f,
		0.0f, -300.0f, 0.0f, 1.0f,
		0.0f, 300.0f, 0.0f, 1.0f,
		// varfuri pentru dreptunghi
		-50.0f,  -50.0f, 0.0f, 1.0f,
		50.0f, -50.0f, 0.0f, 1.0f,
		50.0f,  50.0f, 0.0f, 1.0f,
		-50.0f,  50.0f, 0.0f, 1.0f,
	};

	// culorile varfurilor din colturi
	GLfloat Colors[] = {
	  1.0f, 0.0f, 0.0f, 1.0f,
	  0.0f, 1.0f, 0.0f, 1.0f,
	  0.0f, 0.0f, 1.0f, 1.0f,
	  1.0f, 0.0f, 0.0f, 1.0f,
	};

	// se creeaza un buffer nou
	glGenBuffers(1, &VboId);
	// este setat ca buffer curent
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	// punctele sunt "copiate" in bufferul curent
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	// se creeaza / se leaga un VAO (Vertex Array Object) - util cand se utilizeaza mai multe VBO
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	// se activeaza lucrul cu atribute; atributul 0 = pozitie
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// un nou buffer, pentru culoare
	glGenBuffers(1, &ColorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	// atributul 1 =  culoare
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
}



void DestroyVBO(void)
{
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &ColorBufferId);
	glDeleteBuffers(1, &VboId);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId);
}



void CreateShaders(void)
{
	ProgramId = LoadShaders("03_02_Shader.vert", "03_02_Shader.frag");
	glUseProgram(ProgramId);
}



void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}



void Initialize(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	CreateVBO();
	CreateShaders();
	codColLocation = glGetUniformLocation(ProgramId, "codCuloare");
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
}



void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	/*
		Declare the matrix for translations and rotations
	*/

	resizeMatrix = glm::ortho(-width, width, -height, height);							// scalam, "aducem" scena la "patratul standard" [-1,1]x[-1,1] 

	matrTranslAsphalt = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 60.0, 0.0));		// asfalt + linie discontinua

	matrTransl1 = glm::translate(glm::mat4(1.0f), glm::vec3(i1, k1, 0.0));				// racheta			= car1 -- controleaza translatia de-a lungul lui Ox
	matrTransl2 = glm::translate(glm::mat4(1.0f), glm::vec3(i2, 0.0, 0.0));				// masina jos slow	= car2 -- controleaza translatia de-a lungul lui Ox
	matrTransl3 = glm::translate(glm::mat4(1.0f), glm::vec3(i3, 120.0, 0.0));			// masina sus		= car3 -- controleaza translatia de-a lungul lui Ox, dar invers
	matrTransl4 = glm::translate(glm::mat4(1.0f), glm::vec3(i1+45, k1, 0.0));

	matrTransl5 = glm::translate(glm::mat4(1.0f), glm::vec3(i1 - 18, k1-10, 0.0));			// moving rear lights for car 1
	matrTransl6 = glm::translate(glm::mat4(1.0f), glm::vec3(i1 - 18, k1+10, 0.0));			// moving rear lights for car 1

	matrTransl7 = glm::translate(glm::mat4(1.0f), glm::vec3(i2 - 18, -10.0, 0.0));			// moving rear lights for car 2
	matrTransl8 = glm::translate(glm::mat4(1.0f), glm::vec3(i2 - 18, 10.0, 0.0));			// moving rear lights for car 2

	matrTransl9 = glm::translate(glm::mat4(1.0f), glm::vec3(i3 + 18, 110.0, 0.0));			// moving rear lights for car 3
	matrTransl10 = glm::translate(glm::mat4(1.0f), glm::vec3(i3 + 18, 130.0, 0.0));			// moving rear lights for car 3


	matrScaleAsphalt = glm::scale(glm::mat4(1.0f), glm::vec3(10.1, 2.3, 0.0));				// utila la desenarea asfaltului
	matrScaleBackground = glm::scale(glm::mat4(1.0f), glm::vec3(50.0, 50.0, 0.0));			// utila la desenarea background-ului
	matrScaleCars = glm::scale(glm::mat4(1.0f), glm::vec3(0.4, 0.3, 0.0));					// utila la desenarea masinilor
	matrScaleLights = glm::scale(glm::mat4(1.0f), glm::vec3(0.2, 0.1, 0.0));				// utila la desenarea luminilor
	matrScaleBeacon1 = glm::scale(glm::mat4(1.0f), glm::vec3(0.15, 0.15, 0.0));				// utila la desenarea luminilor
	matrScaleBeacon2 = glm::scale(glm::mat4(1.0f), glm::vec3(0.1, 0.1, 0.0));				// utila la desenarea luminilor
	matrScaleStops = glm::scale(glm::mat4(1.0f), glm::vec3(0.05, 0.05, 0.0));

	matrRot = glm::rotate(glm::mat4(1.0f), 1.57f, glm::vec3(0.0, 0.0, 0.5));				// utilizat la torirea luminilor
	matrRotBeacon1 = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0, 0.0, 0.5));
	matrRotBeacon2 = glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(0.0, 0.0, 0.5));

	

	/*
		Start drawing
	*/

	myMatrix = resizeMatrix * matrScaleBackground;											// Matrix for the background
	colorCode = 2;																				// Set the color
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glUniform1i(codColLocation, colorCode);
	glDrawArrays(GL_POLYGON, 4, 4);


	myMatrix = resizeMatrix * matrTranslAsphalt * matrScaleAsphalt;							// Matrix for the asphalt
	colorCode = 4;																				// Set the color
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glUniform1i(codColLocation, colorCode);
	glDrawArrays(GL_POLYGON, 4, 4);


	myMatrix = resizeMatrix * matrTranslAsphalt;											// Matrix for the traffic lanes
	colorCode = 5;																				// Set the color
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glUniform1i(codColLocation, colorCode);
	glLineStipple(50, 0xAAAA);
	glEnable(GL_LINE_STIPPLE);
	glDrawArrays(GL_LINES, 0, 2);


	myMatrix = resizeMatrix * matrTransl1 * matrScaleCars;									// Matrix for car1 = the rocket
	colorCode = 7;																				// Set the color
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glUniform1i(codColLocation, colorCode);
	glDrawArrays(GL_POLYGON, 4, 4);

	myMatrix = resizeMatrix * matrTransl1 * matrRotBeacon1 * matrScaleBeacon1;					// Matrix for car1 beacon1 = the rocket
	colorCode = 1;																				// Set the color
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glUniform1i(codColLocation, colorCode);
	glDrawArrays(GL_POLYGON, 4, 4);

	myMatrix = resizeMatrix * matrTransl1 * matrRotBeacon2 * matrScaleBeacon2;					// Matrix for car1 beacon2 = the rocket
	colorCode = 6;																				// Set the color
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glUniform1i(codColLocation, colorCode);
	glDrawArrays(GL_POLYGON, 4, 4);

	myMatrix = resizeMatrix * matrTransl5 * matrScaleStops;									// Matrix for car1 stop1 = the rocket
	colorCode = 6;																				// Set the color
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glUniform1i(codColLocation, colorCode);
	glDrawArrays(GL_POLYGON, 4, 4);


	myMatrix = resizeMatrix * matrTransl6 * matrScaleStops;									// Matrix for car1 stop2 = the rocket
	colorCode = 6;																				// Set the color
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glUniform1i(codColLocation, colorCode);
	glDrawArrays(GL_POLYGON, 4, 4);


	myMatrix = resizeMatrix * matrTransl2 * matrScaleCars;									// Matrix for car2 = the lower slower one
	colorCode = 1;																				// Set the color
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glUniform1i(codColLocation, colorCode);
	glDrawArrays(GL_POLYGON, 4, 4);

	myMatrix = resizeMatrix * matrTransl7 * matrScaleStops;									// Matrix for car2 stop1 = the rocket
	colorCode = 6;																				// Set the color
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glUniform1i(codColLocation, colorCode);
	glDrawArrays(GL_POLYGON, 4, 4);


	myMatrix = resizeMatrix * matrTransl8 * matrScaleStops;									// Matrix for car2 stop2 = the rocket
	colorCode = 6;																				// Set the color
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glUniform1i(codColLocation, colorCode);
	glDrawArrays(GL_POLYGON, 4, 4);

	myMatrix = resizeMatrix * matrTransl3 * matrScaleCars;									// Matrix for car3 = the upper normal one
	colorCode = 1;																				// Set the color
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glUniform1i(codColLocation, colorCode);
	glDrawArrays(GL_POLYGON, 4, 4);

	myMatrix = resizeMatrix * matrTransl9 * matrScaleStops;									// Matrix for car3 stop1 = the rocket
	colorCode = 6;																				// Set the color
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glUniform1i(codColLocation, colorCode);
	glDrawArrays(GL_POLYGON, 4, 4);


	myMatrix = resizeMatrix * matrTransl10 * matrScaleStops;									// Matrix for car3 stop2 = the rocket
	colorCode = 6;																				// Set the color
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glUniform1i(codColLocation, colorCode);
	glDrawArrays(GL_POLYGON, 4, 4);

	myMatrix = resizeMatrix * matrTransl4 * matrRot *matrScaleLights ;									// Matrix for lights car1 = the rocket
	colorCode = 3;																				// Set the color
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glUniform1i(codColLocation, colorCode);
	glDrawArrays(GL_TRIANGLES, 3, 3);



	glutSwapBuffers();
	glFlush();
}



void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}



int main(int argc, char* argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1400, 600);
	glutCreateWindow("Project1 - Zabava Claudiu & Baciu Daniel - lab4");

	glewInit();
	Initialize();

	glutDisplayFunc(RenderFunction);
	glutMouseFunc(keyPress);

	glutCloseFunc(Cleanup);
	glutMainLoop();
}

