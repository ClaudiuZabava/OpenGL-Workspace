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

int codCol;
float PI = 3.141592, angle = 0, angle2 = 0;
float tx = 0; float ty = 0;
float width = 450, height = 300;
float i = -350.0, k = 350.0, alpha = 0.0, step = 1.2, beta = 0.002, alpha2 = 0.0, step2 = 1.2;
glm::mat4
myMatrix, resizeMatrix, matrTransl, matrTransl2, matrTransl3, matrScale1, matrScale2, matrScale3, matrScale4, matrRot, matrDepl;

void displayMatrix()
{
	for (int ii = 0; ii < 4; ii++)
	{
		for (int jj = 0; jj < 4; jj++)
			cout << myMatrix[ii][jj] << "  ";
		cout << endl;
	};
	cout << "\n";
};

void miscad(void)
{
	i = i + alpha;
	if (i > 350.0)
		alpha = 0.0;
	else if (i < -350.0)
		alpha = step;
	angle = angle - beta;
	k = k + alpha2;
	if (k > 350.0)
		alpha2 = -step2;
	else if (k < -350.0)
		alpha2 = 0.0;
	angle2 = angle2 + beta;
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			alpha = step;
			alpha2 = -step;
		}
		glutIdleFunc(miscad);
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

	resizeMatrix = glm::ortho(-width, width, -height, height); // scalam, "aducem" scena la "patratul standard" [-1,1]x[-1,1] --- ?? nu stiu ce vrea sa insemne asta
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(i, 0.0, 0.0)); // controleaza translatia de-a lungul lui Ox
	matrTransl2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 60.0, 0.0)); // controleaza translatia de-a lungul lui Ox
	matrTransl3 = glm::translate(glm::mat4(1.0f), glm::vec3(k, 120.0, 0.0)); // controleaza translatia de-a lungul lui Ox, dar invers pt masina din sens opus
	matrScale3 = glm::scale(glm::mat4(1.0f), glm::vec3(10.1, 2.3, 0.0)); // desen asfalt
	matrScale4 = glm::scale(glm::mat4(1.0f), glm::vec3(50.0, 50.0, 0.0)); // desen iarba
	matrScale1 = glm::scale(glm::mat4(1.0f), glm::vec3(1.1, 0.3, 0.0)); // desen masina




	// Matricea pentru ASFALT
	myMatrix = resizeMatrix  * matrScale4;
	// Culoarea
	codCol = 2;
	// Transmitere variabile uniforme
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glUniform1i(codColLocation, codCol);
	// Apelare DrawArrays
	glDrawArrays(GL_POLYGON, 4, 4);

	// Matricea pentru ASFALT
	myMatrix = resizeMatrix * matrTransl2 * matrScale3;
	// Culoarea
	codCol = 4;
	// Transmitere variabile uniforme
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glUniform1i(codColLocation, codCol);
	// Apelare DrawArrays
	glDrawArrays(GL_POLYGON, 4, 4);


	// Marcaj linie discontinua
	myMatrix = resizeMatrix * matrTransl2;
	// Culoarea
	codCol = 5;
	// Transmitere variabile uniforme
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glUniform1i(codColLocation, codCol);
	// Apelare DrawArrays
	glLineStipple(50, 0xAAAA); 
	glEnable(GL_LINE_STIPPLE);
	glDrawArrays(GL_LINES, 0, 2);

	// Matricea pentru masina albastra de jos
	myMatrix = resizeMatrix * matrTransl * matrScale1;
	// Culoarea
	codCol = 1;
	// Transmitere variabile uniforme
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glUniform1i(codColLocation, codCol);
	// Apelare DrawArrays
	glDrawArrays(GL_POLYGON, 4, 4);

	// Matricea pentru masina albastra de sus
	myMatrix = resizeMatrix * matrTransl3 * matrScale1;
	// Culoarea
	codCol = 1;
	// Transmitere variabile uniforme
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glUniform1i(codColLocation, codCol);
	// Apelare DrawArrays
	glDrawArrays(GL_POLYGON, 4, 4);

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
	glutInitWindowSize(800, 600);
	glutCreateWindow("Depasire");
	glewInit();
	Initialize();
	glutDisplayFunc(RenderFunction);
	glutMouseFunc(mouse);
	glutCloseFunc(Cleanup);
	glutMainLoop();
}

