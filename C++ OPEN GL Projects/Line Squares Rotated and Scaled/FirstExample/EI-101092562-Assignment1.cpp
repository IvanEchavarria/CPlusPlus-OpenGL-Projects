
///////////////////////////////////////////////////////////////////////
//
// EI-101092562-Assignment1.cpp by Ivan Echavarria based on Galal Hassan triangles.cpp example
//
// Assigment 1 submission
//
// Description:
// Program creates 10 squares, each square odd squares are scaled down while even squares are scaled and rotated 45 degrees ( Square count is from 1 to 10)
//
///////////////////////////////////////////////////////////////////////

using namespace std;

#include <string>
#include <iostream>
#include "stdlib.h"
#include <ctime>
#include "time.h"
#include "vgl.h"
#include "LoadShaders.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>



enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
//enum Attrib_IDs { vPosition = 0 , vColor = 1 };

int numberOfBuffers = 10;
float angle = 45.0f;
float viewDistance = -4;

GLuint *VAOs = new GLuint [numberOfBuffers];
GLuint *Color_VBO = new GLuint[numberOfBuffers];
GLuint *VertexBufferO = new GLuint[numberOfBuffers];

/*GLuint VAOs[numberOfBuffers];
GLuint Color_VBO[numberOfBuffers];
GLuint VertexBufferO[numberOfBuffers];*/

GLint  vPosition;
GLint  vColor;
GLint  Model;

const GLuint NumVertices = 4;
GLfloat scale = 60.f;
float randomRedColor;
float randomGreenColor;
float randomBlueColor;


GLfloat vertices[] =
{
	// X      Y
 	-0.5f, -0.5f,
	-0.5f,  0.5f,
	 0.5f,  0.5f,
	 0.5f, -0.5f
};

glm::mat4 ModelMatrix;
glm::mat4 RotationMatrix;
glm::mat4 ScaleMatrix = glm::mat4(1.0f);

void setAngle(float newAngle)
{
	angle = newAngle;
}

void init(void)
{
	

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
	glGenVertexArrays(numberOfBuffers, VAOs);

	//Generating two buffers, one is used to store the coordinates of the vertices
	//The other one is not used. Just wanted to show that we can allocate as many as buffers, some of which might left unused.
	glGenBuffers(numberOfBuffers, VertexBufferO);	

	//Generate VBO for color
	glGenBuffers(numberOfBuffers, Color_VBO);
	
	

	for (int i = 0; i < numberOfBuffers; i++)
	{
		randomRedColor = (rand() % 11) / 10.0f;
		randomGreenColor = (rand() % 11) / 10.0f;
		randomBlueColor = (rand() % 11) / 10.0f;

		GLfloat Colors[12] = 
		{
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f
		};
		
		glBindVertexArray(VAOs[i]);		
		//Bind vertex data
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferO[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		//First Parameter in the glVertexAtrribPointer is an index that gets mapped to the vertex shader and has to be unique
		glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		for (int j = 0; j < 12; j++)
		{			
			if (j % 3 == 0)
			{
				Colors[j] *= randomRedColor;
			}
			else if (j % 3 == 1)
			{
				Colors[j] *= randomGreenColor;
			}
			else if (j % 3 == 2)
			{
				Colors[j] *= randomBlueColor;
			}
			
		}

		//bind color data
		glBindBuffer(GL_ARRAY_BUFFER, Color_VBO[i]);
		glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), &Colors, GL_STATIC_DRAW);		
		glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		glEnableVertexAttribArray(vPosition);
		glEnableVertexAttribArray(vColor);
	}	

	glBindAttribLocation(program, 0, "vPosition");
	glBindAttribLocation(program, 1, "vColor");
		
}


//---------------------------------------------------------------------
//
// display
//

void
display(void)
{
	glClearColor(1.0f, 1.0f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	//numberOfBuffers
	for (int i = 0; i < numberOfBuffers; i++)
	{
		glBindVertexArray(VAOs[i]);
		//Create matrix rotation of 45 radians and Scale on X and Y
		ScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale));
		RotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle * (i%2)), glm::vec3(0.0, 0.0, 1.0));

		//Make the model matrix equal to the rotation transform
		ModelMatrix =  RotationMatrix * ScaleMatrix ;

		//Bind the matrix to the vertex shader 
		glUniformMatrix4fv(Model, 1, GL_FALSE, glm::value_ptr(ModelMatrix));

		//Ordering the GPU to start the pipeline
		glDrawArrays(GL_LINE_LOOP, 0, NumVertices);

		//Scale each square to 70% of its previous size
		scale *= 0.7f;
		
		
	}

	glFlush();
}


//---------------------------------------------------------------------
//
// main
//

int main(int argc, char** argv)
{
	srand (time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(900, 900);
	glutCreateWindow("Echavarria, Ivan, 101092562");

	float newAngle;

	cout << "Enter the float desired angle : ";

	cin >> newAngle;

	setAngle(float(newAngle));

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);

	glutMainLoop();
	
	

}
