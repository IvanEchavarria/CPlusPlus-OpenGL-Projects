//*********************************************************************************
// EI-101092562-Assignment3.cpp by Ivan Echavarria based on Gala Hassan's 3D printer.
//
// Assignment 3 submission.
//
// Description:
//   Running this software will print a rubics cube to the screen
//*********************************************************************************


#include "stdlib.h"
#include "iostream"
#include "SOIL.h"
#include "time.h"
#include "vgl.h"
#include "LoadShaders.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"


#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)
void keyOperations();

GLuint gVAO;
GLuint gVBO;
GLuint IBO;
GLuint gVBO_Color;
GLuint TBO;

GLuint cube_tex = 0;


GLuint MatrixID;
glm::mat4 MVP;
glm::mat4 View;
glm::mat4 Projection;

glm::vec3 currentCamPos;
glm::vec3 currentCamVel;

GLint width, height;

//Load texture
unsigned char* image = SOIL_load_image("./Images/rubiksTexture.png", &width, &height, 0, SOIL_LOAD_RGB);



GLfloat cube[] =
{   //X       Y       Z
   -0.65f, -0.65f, 0.65f,   //V0
   0.65f, -0.65f, 0.65f,   //V1
   0.65f,  0.65f, 0.65f,   //V2
   -0.65f, -0.65f, 0.65f,   //V3   Front face
   0.65f,  0.65f, 0.65f,
	-0.65f,  0.65f, 0.65f,

	0.65f,  -0.65f,  0.65f,   //V8
	0.65f,  -0.65f, -0.65f,   //V9
	0.65f,  0.65f, -0.65f,   //V10
	0.65f, -0.65f,  0.65f,   //V11 Right Face
	0.65f,  0.65f, -0.65f,
	0.65f,  0.65f, 0.65f,

   -0.65f,  -0.65f, -0.65f,
   0.65f,  0.65f, -0.65f,
   0.65f, -0.65f, -0.65f,
   -0.65f, -0.65f, -0.65f,   //Back Face
    -0.65f,  0.65f, -0.65f,
	0.65f,  0.65f, -0.65f,

   -0.65f,  0.65f, 0.65f,
   0.65f,  0.65f, 0.65f,
   0.65f,  0.65f, -0.65f,
   -0.65f,  0.65f, 0.65f,   //Top Face
   0.65f,  0.65f, -0.65f,
   -0.65f,  0.65f, -0.65f,

   -0.65f, -0.65f,  0.65f,
   -0.65f,  0.65f, -0.65f,
   -0.65f,  -0.65f, -0.65f,
   -0.65f,  -0.65f,  0.65f,   //Left Face	
   -0.65f,  0.65f, 0.65f,
    -0.65f,  0.65f, -0.65f,

   -0.65f,  -0.65f, 0.65f,   //V20
   0.65f,  -0.65f, -0.65f,   //V21
   0.65f,  -0.65f, 0.65f,   //V22
   -0.65f,  -0.65f, 0.65f,  //V23  Bottom Face
   -0.65f,  -0.65f, -0.65f,
   0.65f,  -0.65f, -0.65f,


};


//Full texture on each face coordinates
/*GLfloat textureCoords[] =
{
	0,1,
	1,1,
	1,0,
	0,1,
	1,0,
	0,0,// Front face

	0,1,
	1,1,
	1,0,
	0,1,
	1,0,
	0,0,// Right Face

	0,1,
	1,0,
	1,1,
	0,1,
	0,0,
	1,0,// Back Face

	0,1,
	1,1,
	1,0,
	0,1,
	1,0,
	0,0,// Top Face

	0,1,
	1,0,
	1,1,
	0,1,
	0,0,
	1,0,//Left Face

	0,1,
	1,0,
	1,1,
	0,1,
	0,0,
	1,0,// Bottom Face
};*/


// Each face with texture
GLfloat textureCoords[] =
{
	0.25,0.66,
	0.50,0.66,
	0.50,0.33,
	0.25,0.66,
	0.50,0.33,
	0.25,0.33,// Front face

	0.50,0.66,
	0.75,0.66,
	0.75,0.33,
	0.50,0.66,
	0.75,0.33,
	0.50,0.33,// Right Face

	0.75,0.66,
	1,0.33,
	1,0.66,
	0.75,0.66,
	0.75,0.33,
	1,0.33,// Back Face

	0.25,0.33,
	0.50,0.33,
	0.50,0,
	0.25,0.33,
	0.50,0,
	0.25,0,// Top Face

	0,0.66,
	0.25,0.33,
	0.25,0.66,
	0,0.66,
	0,0.33,
	0.25,0.33,//Left Face

	0.25,1,
	0.50,0.66,
	0.50,1,
	0.25,1,
	0.25,0.66,
	0.50,0.66,// Bottom Face
};

/*GLfloat cubeColors[] =
{
	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,
	

	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,

	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,

	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,

	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,

	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,
	1.0f, 0.0f, 0.7f,
};*/



float angle = 45.0f;

float upDown = 8.25f;

bool  changeDirection = false;


int frame = 0, currentTime, timebase = 0;
float deltaTime = 0;
bool keyStates[256] = {}; // Create an array of boolean values of length 256 (0-255)

void init(void) {
	//Specifying the name of vertex and fragment shaders.
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
	{ GL_FRAGMENT_SHADER, "triangles.frag" },
	{ GL_NONE, NULL }
	};

	//Loading and compiling shaders
	GLuint program = LoadShaders(shaders);
	glUseProgram(program);    //My Pipeline is set up

							  // Should I be generating some VAOs here? I don't know what I am doing, I am just an Intern

	
	glGenVertexArrays(1, &gVAO);
	glGenBuffers(1, &gVBO);
	glGenBuffers(1, &IBO);
	//glGenBuffers(1, &gVBO_Color);
	glGenBuffers(1, &TBO);
	glGenTextures(1, &cube_tex);
	glActiveTexture(GL_TEXTURE0);
	


							  // Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(program, "MVP");

	// Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	currentCamPos = glm::vec3(0.0f, 0.0f, 10.0f);
	currentCamVel = glm::vec3(0.0f);

	// Camera matrix
	View = glm::lookAt(
		currentCamPos, // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// One of my team members said I should start defining my objects here. No one is going to see these comments right? We are dont worry
	glBindVertexArray(gVAO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));	

	//glBindBuffer(GL_ARRAY_BUFFER, gVBO_Color);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(cubeColors), cubeColors, GL_STATIC_DRAW);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//Texture Buffer
	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//Texture Object
	glBindTexture(GL_TEXTURE_2D, cube_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image); // bind image to texture object
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glUniform1i(glGetUniformLocation(program, "texture0"), 0);


	glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindAttribLocation(program, 0, "vertex_position");	
	//glBindAttribLocation(program, 1, "vertex_colour");
	glBindAttribLocation(program, 2, "vertex_texture");

}

void transformObject(glm::vec3 scale, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation) {
	glm::mat4 Model;
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, translation);
	Model = glm::rotate(Model, glm::radians(rotationAngle), rotationAxis);
	Model = glm::scale(Model, scale);

	MVP = Projection * View * Model;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
}

//---------------------------------------------------------------------
//
// display
//


void display(void) {
	keyOperations();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	angle = glutGet(GLUT_ELAPSED_TIME) / 1000.0 * 45;
	
	glBindVertexArray(gVAO);
	transformObject(glm::vec3(1.0f), glm::vec3(0.0f, 1.0f, 0.0f), angle, glm::vec3(3.0f, 0.0f, 0.0f));	
	glDrawArrays(GL_TRIANGLES, 0, 36);


	glBindVertexArray(gVAO);
	transformObject(glm::vec3(1.0f), glm::vec3(0.0f, 1.0f, 0.0f), -angle, glm::vec3(-3.0f, 0.0f, 0.0f));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	

	glutSwapBuffers();
	deltaTime = (glutGet(GLUT_ELAPSED_TIME) - currentTime) / 1000.0f;
	currentTime = glutGet(GLUT_ELAPSED_TIME);
}

void keyDown(unsigned char key, int x, int y) {
	keyStates[key] = true; // Set the state of the current key to pressed
}

void keyUp(unsigned char key, int x, int y) {
	keyStates[key] = false; // Release the state of the current key to pressed
}

void keyOperations(void) {
	float cameraSpeed = 10.0f;
	if (keyStates['w']) { // If the 'w' key has been pressed
		currentCamPos.z -= cameraSpeed * (deltaTime);
	}
	if (keyStates['s']) { // If the 's' key has been pressed
		currentCamPos.z += cameraSpeed * (deltaTime);
	}
	if (keyStates['a']) { // If the 'a' key has been pressed
		currentCamPos.x -= cameraSpeed * (deltaTime);
	}
	if (keyStates['d']) { // If the 'd' key has been pressed
		currentCamPos.x += cameraSpeed * (deltaTime);
	}
	if (keyStates['r']) { // If the 'r' key has been pressed
		currentCamPos.y += cameraSpeed * (deltaTime);
	}
	if (keyStates['f']) { // If the 'f' key has been pressed
		currentCamPos.y -= cameraSpeed * (deltaTime);
	}
	View = glm::lookAt(
		currentCamPos, // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
}

void keyboardFun(unsigned char key, int x, int y) {
	float cameraSpeed = 10.0f;
	switch (key) {
	case 'w':
		// call a function
		currentCamPos.z -= cameraSpeed * (deltaTime);
		std::cout << deltaTime << std::endl;
		break;
	case 's':
		currentCamPos.z += cameraSpeed * (deltaTime);
		// call a function
		break;
	case 'a':
		currentCamPos.x -= cameraSpeed * (deltaTime);
		// call a function
		break;
	case 'd':
		currentCamPos.x += cameraSpeed * (deltaTime);
		// call a function
		break;
	case 'r':
		currentCamPos.y += cameraSpeed * (deltaTime);
		// call a function
		break;
	case 'f':
		currentCamPos.y -= cameraSpeed * (deltaTime);
		// call a function
		break;

	default:
		break;
	}

}

void idle() {

}

void Timer(int id) {
	glutPostRedisplay();
	glutTimerFunc(15, Timer, 0);
}

//---------------------------------------------------------------------
//
// main
//
int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Echavarria, Ivan , 101092562");

	glewExperimental = true;
	glewInit();    //Initializes the glew and prepares the drawing pipeline.

	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
	glEnable(GL_DEPTH_TEST);
	init();

	glutTimerFunc(15, Timer, 0);

	glutDisplayFunc(display);

	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);

	glutIdleFunc(idle);

	glutMainLoop();
}




