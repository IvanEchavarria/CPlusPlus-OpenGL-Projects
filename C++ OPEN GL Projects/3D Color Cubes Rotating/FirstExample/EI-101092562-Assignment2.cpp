
///////////////////////////////////////////////////////////////////////
//
// EI-101092562-Assignment2.cpp by Ivan Echavarria based on Galal Hassan triangles.cpp example
//
// Assigment 2 submission
//
// Description:
// Program Creates two 3D Triangles floating in outerspace
//
///////////////////////////////////////////////////////////////////////

using namespace std;

#include <string>
#include <iostream>
#include "stdlib.h"

#include <gl\glew.h>
#include <ctime>
#include "time.h"
#include "vgl.h"
#include "LoadShaders.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "GL\glut.h"
#include <glm/gtc/type_ptr.hpp>




int numberOfBuffers = 1;
float angle = 45.0f;
float viewDistance = -4;


GLuint  VAOs;
GLuint  Color_VBO;
GLuint  VertexBufferO;
GLuint  IBO;

GLint  vPosition;
GLint  vColor;
GLuint  Model;

const GLuint NumVertices = 4;

float cameraViewX = 0.0f;
float cameraViewY = 45.0f;
float cameraViewUpDown = -7.0f;


GLfloat vertices[] =
{
	// X      Y     Z
 	-0.45f, -0.45f, 0.45f, //0
	 0.45f, -0.45f, 0.45f, //1
	 0.45f,  0.45f, 0.45f, //2
	-0.45f,  0.45f, 0.45f, //3

	-0.45f, -0.45f, -0.45f, //4
	 0.45f, -0.45f, -0.45f, //5
	 0.45f,  0.45f, -0.45f, //6
	-0.45f,  0.45f, -0.45f  //7
};

GLushort cubeIndices[]=
{
	//front
	0,1,2,
	0,2,3,

	//Back
	4,5,6,
	4,6,7,

	//right
	1, 5 ,6,
	1, 6 ,2,

	//left
	0, 4 ,7,
	0, 7 ,3,
	
	//bot
	0,1,5,
	0,5,4,

	//top
	3,2,6,
	3,6,7

};

glm::mat4 ModelMatrix;
glm::mat4 RotationMatrix;
glm::mat4 View;
glm::mat4 Projection;
glm::mat4 ScaleMatrix = glm::mat4(1.0f);
glm::mat4 Translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.45, 0.0)); // push the view a little further

void setAngle(float newAngle)
{
	angle = newAngle;
}

void init(void)
{
	
	glEnable(GL_DEPTH_TEST);
	//Specifying the name of vertex and fragment shaders.
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	//Loading and compiling shaders
	GLuint program = LoadShaders(shaders);
	glUseProgram(program);	//My Pipeline is set up

	//Link vertex data and attributes
	vPosition = glGetAttribLocation(program, "vPosition");
	vColor = glGetAttribLocation(program, "vColor");
	Model = glGetUniformLocation(program, "Model");	

	
	//Generate VAOS
	glGenVertexArrays(numberOfBuffers, &VAOs);

	glGenBuffers(numberOfBuffers, &VertexBufferO);	

	//Generate VBO for color
	glGenBuffers(numberOfBuffers, &Color_VBO);

	//Generate IBO for INDEX BUFFER
	glGenBuffers(numberOfBuffers, &IBO);
	


	GLfloat Colors[24] = 
	{
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,

		0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f
	};
		
	glBindVertexArray(VAOs);		
	//Bind vertex data
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//First Parameter in the glVertexAtrribPointer is an index that gets mapped to the vertex shader and has to be unique
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//Bind IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

	
    //bind color data
	glBindBuffer(GL_ARRAY_BUFFER, Color_VBO);
	glBufferData(GL_ARRAY_BUFFER, 24*sizeof(float), Colors, GL_STATIC_DRAW);		
	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	

	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(vColor);
		

	glBindAttribLocation(program, 0, "vPosition");
	glBindAttribLocation(program, 1, "vColor");

		
}

void drawSquares()
{
	glm::mat4 Translate = glm::translate(glm::mat4(1.0f), glm::vec3(cameraViewX, 0.45, 0.0)); // push the view a little further

	Projection = glm::perspective(glm::radians(cameraViewY), 1.0f * (900.0f / 900.0f), 0.1f, 100.0f);
	//GLfloat scale = 1.f;
	View = glm::lookAt(
			glm::vec3(0.0f, cameraViewUpDown, -7.0f), // Camera is at 4,3,3 om world space
			glm::vec3(0, 0, 0),    // Looking at the origin
			glm::vec3(0, 1, 0)     // HEads up, set to 0,-1,0 to look upside down
	);

	angle = glutGet(GLUT_ELAPSED_TIME) / 1000.0 * 45;
	//glBindVertexArray(VAOs);
	//Create matrix rotation of 45 radians and Scale on X and Y

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	int size;  
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

	glm::mat4 ModelMatrixOp = Translate;
	ModelMatrixOp = glm::rotate(ModelMatrixOp, glm::radians(angle), glm::vec3(0.0, 1.0, 0.0));
	ModelMatrix = Projection * View * ModelMatrixOp;
	//Bind the matrix to the vertex shader 
	glUniformMatrix4fv(Model, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

	glutSwapBuffers();

	ModelMatrixOp = glm::translate(glm::mat4(1.0f), glm::vec3(cameraViewX, -0.45, 0.0));
	ModelMatrixOp = glm::rotate(ModelMatrixOp, glm::radians(-angle), glm::vec3(0.0, 1.0, 0.0));
	ModelMatrix = Projection * View * ModelMatrixOp;
	glUniformMatrix4fv(Model, 1, GL_FALSE, glm::value_ptr(ModelMatrix));

	glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

	glutSwapBuffers();
}

//---------------------------------------------------------------------
//
// display
//

void
display(void)
{
	
	glDepthFunc(GL_LESS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 0.4f, 1.0f);
	
		
	drawSquares();
	
	glFlush();
}

void Timer(int id)
{
	glutPostRedisplay();
	glutTimerFunc(33, Timer, 0);
}

void KeyDown(unsigned char key, int x, int y)
{
	float cameraSpeed = 1.0f;
	
	switch (key)
	{
	case 'w':
		//Moves the camera in 
		cameraViewY += cameraSpeed;
		break;
	case 's':
		//Moves the camera out
		cameraViewY -= cameraSpeed;
		break;
	case 'a':
		//Move Camera left
		cameraViewX += cameraSpeed;
		break;
	case 'd':
		//Move camera right
		cameraViewX -= cameraSpeed;
		break;
	case 'r':
		//Move the camera up
		cameraViewUpDown += cameraSpeed;
		break;
	case 'f':
		//move the camera down
		cameraViewUpDown -= cameraSpeed;
		break;
	default:
		//Do amazing stuff ( Dont remove because of tahas issues and this is a safe place)
		break;
	}
}



//---------------------------------------------------------------------
//
// main
//

int main(int argc, char** argv)
{
	srand (time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(900, 900);
	glutCreateWindow("Echavarria, Ivan, 101092562");
	glutTimerFunc(33, Timer, 0);
	glutKeyboardFunc(KeyDown);
	
	glewExperimental = true;

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);

	glutMainLoop();
	
	

}
