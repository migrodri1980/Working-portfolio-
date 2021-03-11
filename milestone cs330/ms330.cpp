/*
 * ms330.cpp
 *
 *  Created on: Sep 25,2020
 *      Author: Miguel CP Rodriguez IV
 */

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//SOIL image loader inclusion
#include "SOIL2/SOIL2.h"

using namespace std;

#define WINDOW_TITLE "Milestone 330"

#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version "\n" #Source
#endif

GLint shaderProgram, WindowWidth = 800, WindowHeight = 600;
GLuint VBO, VAO, EBO, texture;
GLfloat degrees = glm::radians(0.0f);

GLfloat cameraRotation = glm::radians(-45.0f);
GLfloat cameraSpeed = 0.0005f; // movement speed per frame


GLchar currentKey; //will store key pressed


//Cube and light color
glm::vec3 lightColor(0.78f, 0.88f, 1.0f); //applied overcast sky like  201,226,255 rbg values equal
glm::vec3 secondLightColor(0.49f, 0.43f, 0.40f); //applied light brown to second color

//Light position and scale
glm::vec3 lightPosition(-1.0f, 0.0f, -3.0f);
glm::vec3 lightScale(0.1f);


glm::vec3 lightStrength(1.0f, 1.0f, 0.25f);//ambient   specular    highlight

//set scale by axis numbers
GLfloat scale_by_y = 1.0f;
GLfloat scale_by_x = 1.0f;
GLfloat scale_by_z = 1.0f;

GLfloat lastMouseX = 400, lastMouseY = 300; // Locks mouse cursor at center of screen
GLfloat mouseXOffset, mouseYOffset, yaw = 0.0f, pitch = 0.0f, zoom = 0.0f; // mouse offset, yaw, and pitch variables
GLfloat sensitivity = 0.005f; // used for mouse/camera rotation sensitivity

//vstate for view state set at perspective intially
int vstate = 1;
//for keymod for glutmodifers
int keymod;

bool checkMotion = false;
bool leftMouseButton = false;
bool rightMouseButton = false;
bool altDown = false;

//Global vector declarations
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, -5.0f); // initial camera position placed 5 units in z
glm::vec3 CameraUpY = glm::vec3(0.0f, 1.0f, 0.0f); // temporary y unit vector
glm::vec3 CameraForwardZ = glm::vec3(0.0f, 0.0f, -1.0f); // temporary z unit vector
glm::vec3 front; // temporary z unit vector for mouse
glm::vec3 cameraRotateAmt;

void UResizeWindow(int,int);
void URenderGraphics(void);
void UCreateShader(void);
void UCreateBuffers(void);
void UMouseMove(int x, int y);
void UMouseClick(int button, int state, int x, int y);
void UGenerateTexture(void);
void UKeyboard(unsigned char key, int x, int y);
void UKeyReleased(unsigned char key, int x, int y);

//VERTEX SHADER SOURCE CODE
const GLchar * vertexShaderSource = GLSL(330,
	layout (location = 0) in vec3 position; //vertex data from vertex attrib pointer 0
	layout (location = 2) in vec2 textureCoordinate;

	out vec2 mobileTextureCoordinate;

	//global variables for the transform matrices
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main(){
		gl_Position = projection * view * model * vec4(position, 1.0f); //transforms vertices to clip coordinates
		mobileTextureCoordinate = vec2(textureCoordinate.x, 1.0f - textureCoordinate.y); //flips the texture horizontal
	}
);

//FRAGMENT SHADER SOURCE CODE
const GLchar * fragmentShaderSource = GLSL(330,
	in vec2 mobileTextureCoordinate;

	out vec4 gpuTexture; //variable to pass color data to the GPU

	uniform sampler2D uTexture; //Useful when working with multiple textures

	void main(){
		gpuTexture = texture(uTexture, mobileTextureCoordinate);
	}
);

//LIGHT VERTEX SHADER CODE
const GLchar * lightVertexShaderSource = GLSL(330,
	layout (location = 0) in vec3 position; //VAP position 0 for vertex position data
	layout (location = 1) in vec3 normal; //VAP position 1 for normals
	layout (location = 2) in vec2 textureCoordinate;

	out vec3 Normal; //for outgoing normals to fragment shader
	out vec3 FragmentPos; // for outgoing color / pixels to fragment shader
	out vec2 mobileTextureCoordinate; // uv coords for texture

	//uniform / global variables for the transform matrices
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

    void main(){
        gl_Position = projection * view * model * vec4(position, 1.0f);//Transforms vertices into clip coordinates
        Normal = mat3(transpose(inverse(model))) * normal; // get normal vectors in world space only and exclude normal translation properties
        FragmentPos = vec3(model * vec4(position, 1.0f)); //Gets fragment / pixel position in world space only (exclude view and projection)
		mobileTextureCoordinate = vec2(textureCoordinate.x, 1.0f - textureCoordinate.y); //flips the texture horizontal
	}
);

//LIGHT FRAGMENT SHADER CODE
const GLchar * lightFragmentShaderSource = GLSL(330,
	in vec3 Normal; //For incoming normals
	in vec3 FragmentPos; //for incoming fragment position
	in vec2 mobileTextureCoordinate;

	out vec4 result; //for outgoing light color to the GPU

	//Uniform / Global variables for object color, light color, light position and camera/view position
	uniform vec3 lightColor;
	uniform vec3 secondLightColor;
	uniform vec3 lightPos;
	uniform vec3 viewPosition;
    uniform vec3 lightStrength;
	uniform sampler2D uTexture; //Useful when working with multiple textures

    void main(){
    	vec3 norm = normalize(Normal); //Normalize vectors to 1 unit
    	vec3 ambient = lightStrength.x * lightColor; //Generate ambient light color
    	vec3 ambientTwo = lightStrength.x * secondLightColor;//Generate second ambient light color
    	vec3 lightDirection = normalize(lightPos - FragmentPos); //Calculate distance (light direction) between light source and fragments/pixels on
    	float impact = max(dot(norm, lightDirection), 0.0); //Calculate diffuse impact by generating dot product of normal and light
    	vec3 diffuse = impact * lightColor; //Generate diffuse light color
    	vec3 viewDir = normalize(viewPosition - FragmentPos); //Calculate view direction
    	vec3 reflectDir = reflect(-lightDirection, norm); //Calculate reflection vector
    	float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), lightStrength.z);
    	vec3 specular = lightStrength.y * specularComponent * lightColor;

    	//Calculate phong result
    	vec3 phongOne = (ambient + diffuse + specular) * vec3(texture(uTexture, mobileTextureCoordinate));

    	// hardcode second light position
    	lightDirection = normalize(vec3(6.0f, 0.0f, -3.0f)- FragmentPos);
    	impact = max(dot(norm, lightDirection), 0.0); //Calculate diffuse impact by generating dot product of normal and light
    	diffuse = impact * secondLightColor; //Generate diffuse light color
    	viewDir = normalize(viewPosition - FragmentPos); //Calculate view direction
    	reflectDir = reflect(-lightDirection, norm); //Calculate reflection vector
    	specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), lightStrength.z);
    	// hardcode second light spec
    	vec3 specularTwo = 0.1f * specularComponent * secondLightColor;

    	vec3 phongTwo = (ambientTwo + diffuse + specularTwo) * vec3(texture(uTexture, mobileTextureCoordinate));

    	result = vec4(phongOne + phongTwo, 1.0f); //Send lighting results to GPU
	}
);


//MAIN PROGRAM
int main(int argc, char * argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutCreateWindow(WINDOW_TITLE);

	glutReshapeFunc(UResizeWindow);

	glewExperimental = GL_TRUE;
		if(glewInit() != GLEW_OK) {
			std::cout << "Failed to initialize GLEW" << std:: endl;
			return -1;
		}

	UCreateShader();

	UCreateBuffers(); // Call Buffer function

	UGenerateTexture();

	glUseProgram(shaderProgram); //Use Shader Program

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color

	glutDisplayFunc(URenderGraphics);
	glutMotionFunc(UMouseMove); //detects mouse movement
	glutMouseFunc(UMouseClick); //detect mouse click
	glutKeyboardFunc(UKeyboard); //detect keypress
	glutKeyboardUpFunc(UKeyReleased);




	glutMainLoop();

	//Destroy Buffer objects once used
	glDeleteVertexArrays(1,&VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	return 0;
}

/*Resize window*/
void UResizeWindow(int w, int h) {
	WindowWidth = w;
	WindowHeight = h;
	glViewport(0, 0, WindowWidth, WindowHeight);
}

/*Renders graphics*/
void URenderGraphics(void) {

	glEnable(GL_DEPTH_TEST); // enable z-depth

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears the screen

	glBindVertexArray(VAO); // Activate vertex array object before rendering and transforming them

	//Transform object
	glm::mat4 model;
	model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f)); //Increase the object size by a scale of 6
	model = glm::rotate(model, degrees, glm::vec3(0.0, 1.0f, 0.0f)); //Rotate the object y -45 degrees
	model = glm::translate(model, glm::vec3(0.0, 0.0f, 0.0f)); //Place the object at the center of the viewport
	model = glm::scale(model, glm::vec3(scale_by_x,scale_by_y,scale_by_z)); // Increase scale of object by scale of xyz

	//transform camera
	glm::mat4 view;
	view = glm::lookAt(cameraPosition, CameraForwardZ, CameraUpY);
	view = glm::rotate(view, cameraRotateAmt.x, glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::rotate(view, cameraRotateAmt.y, glm::vec3(1.0f, 0.0f, 0.0f));

	//transform projection
	glm::mat4 projection;

	if(vstate == 1){
		projection = glm::perspective(45.0f, (GLfloat)WindowWidth / (GLfloat)WindowHeight, 0.1f, 100.0f);

	}else if(vstate == 0){
		projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
	}


	//Retrieves and passes transform matrices to the shader program
	GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
	GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
	GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
	GLint secondLightColorLoc, lightColorLoc, lightPositionLoc, lightStrengthLoc, viewPositionLoc;

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
	lightPositionLoc = glGetUniformLocation(shaderProgram, "lightPos");
    lightStrengthLoc = glGetUniformLocation(shaderProgram, "lightStrength");
    secondLightColorLoc = glGetUniformLocation(shaderProgram, "secondLightColor");
	viewPositionLoc = glGetUniformLocation(shaderProgram, "viewPosition");

	//pass color, light, and camera data to the cube shader programs corresponding uniforms
	glUniform3f(lightColorLoc, lightColor.r, lightColor.g, lightColor.b);
	glUniform3f(secondLightColorLoc, secondLightColor.r, secondLightColor.g, secondLightColor.b);
	glUniform3f(lightPositionLoc, lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform3f(lightStrengthLoc, lightStrength.x, lightStrength.y, lightStrength.z);
	glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	glutPostRedisplay();

	glBindTexture(GL_TEXTURE_2D, texture);

	glDrawElements(GL_TRIANGLES, 600, GL_UNSIGNED_INT, 0); //Draw triangles

	glBindVertexArray(0); // Deactivate vertex array object

	glutSwapBuffers(); // Flips back buffer with front buffer every frame
}

//CREATE SHADER PROGRAMS
void UCreateShader() {
	//Vertex Shader
	GLint vertexShader = glCreateShader(GL_VERTEX_SHADER); // create vertex shader
	glShaderSource(vertexShader, 1, &lightVertexShaderSource, NULL); // attach vertex shader to source code
	glCompileShader(vertexShader); // compile vertex shader

	//Fragment Shader
	GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // create fragment shader
	glShaderSource(fragmentShader, 1, &lightFragmentShaderSource, NULL); // attach fragment shader to source code
	glCompileShader(fragmentShader); // compile fragment shader

	//Shader program
	shaderProgram = glCreateProgram(); // create shader program and return ID
	glAttachShader(shaderProgram, vertexShader); // attach vertex shader to program
	glAttachShader(shaderProgram, fragmentShader); // attach fragment shader to program
	glLinkProgram(shaderProgram); // link vertex and fragment shaders to program

	//Delete vertex and fragment shaders once linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

//CREAT BUFFERS AND OBJECTS
void UCreateBuffers() {
	//Position and color data
	GLfloat vertices[] = {
			//X   //Y   //Z			//NORMALS			//TEXTURE COOR

			//chair leg one
			0.6f, -1.0f,  0.0f,  0.17f, 0.11f, 0.05f, 	1.0f,  0.0f,	//0
			0.5f, -1.0f,  0.0f,	 0.17f, 0.11f, 0.05f, 	0.75f, 0.0f,	//1
			0.6f, -1.0f, -0.1f,  0.17f, 0.11f, 0.05f, 	0.0f,  0.0f,	//2
			0.5f, -1.0f, -0.1f,  0.17f, 0.11f, 0.05f,	0.5f,  0.0f,	//3

			0.6f,  0.0f,  0.0f,  0.17f, 0.11f, 0.05f,   1.0f,  1.0f,	//4
			0.5f,  0.0f,  0.0f,  0.17f, 0.11f, 0.05f,	0.75f, 1.0f,	//5
 			0.6f,  0.0f, -0.1f,  0.17f, 0.11f, 0.05f, 	0.0f,  1.0f,	//6
			0.5f,  0.0f, -0.1f,  0.17f, 0.11f, 0.05f,	0.5f,  1.0f,	//7

			//chair leg two
			-0.6f, -1.0f,  0.0f,  0.17f, 0.11f, 0.05f, 	1.0f, 0.0f,		//8
			-0.5f, -1.0f,  0.0f,  0.17f, 0.11f, 0.05f, 	0.75f,0.0f,		//9
			-0.6f, -1.0f, -0.1f,  0.17f, 0.11f, 0.05f, 	0.0f, 0.0f,		//10
			-0.5f, -1.0f, -0.1f,  0.17f, 0.11f, 0.05f,	0.5f, 0.0f,		//11

			-0.6f,  0.0f,  0.0f,  0.17f, 0.11f, 0.05f,  1.0f, 1.0f,		//12
			-0.5f,  0.0f,  0.0f,  0.17f, 0.11f, 0.05f,	0.75f,1.0f,		//13
 			-0.6f,  0.0f, -0.1f,  0.17f, 0.11f, 0.05f,  0.0f, 1.0f,		//14
			-0.5f,  0.0f, -0.1f,  0.17f, 0.11f, 0.05f,	0.5f, 1.0f,		//15

			//chair leg three
			0.6f, -1.0f, -0.9f,  0.17f, 0.11f, 0.05f,	1.0f, 0.0f,		//16
			0.5f, -1.0f, -0.9f,	 0.17f, 0.11f, 0.05f,	0.75f,0.0f,		//17
			0.6f, -1.0f, -1.0f,  0.17f, 0.11f, 0.05f, 	0.0f, 0.0f,		//18
			0.5f, -1.0f, -1.0f,  0.17f, 0.11f, 0.05f,	0.5f, 0.0f,		//19

			0.6f,  0.0f, -0.9f,   0.17f, 0.11f, 0.05f, 	1.0f, 1.0f,		//20
			0.5f,  0.0f, -0.9f,   0.17f, 0.11f, 0.05f,	0.75f,1.0f,		//21
 			0.6f,  0.0f, -1.0f,   0.17f, 0.11f, 0.05f, 	0.0f, 1.0f, 	//22
			0.5f,  0.0f, -1.0f,   0.17f, 0.11f, 0.05f, 	0.5f, 1.0f,		//23

			//chair leg four
			-0.6f, -1.0f, -0.9f,  0.17f, 0.11f, 0.05f,	1.0f, 0.0f,		//24
			-0.5f, -1.0f, -0.9f,  0.17f, 0.11f, 0.05f,	0.75f,0.0f,		//25
			-0.6f, -1.0f, -1.0f,  0.17f, 0.11f, 0.05f,	0.0f, 0.0f, 	//26
			-0.5f, -1.0f, -1.0f,  0.17f, 0.11f, 0.05f,	0.5f, 0.0f,		//27

			-0.6f,  0.0f, -0.9f,  0.17f, 0.11f, 0.05f, 	1.0f, 1.0f,		//28
			-0.5f,  0.0f, -0.9f,  0.17f, 0.11f, 0.05f, 	0.75f,1.0f,		//29
 			-0.6f,  0.0f, -1.0f,  0.17f, 0.11f, 0.05f, 	0.0f, 1.0f,		//30
			-0.5f,  0.0f, -1.0f,  0.17f, 0.11f, 0.05f,  0.5f, 1.0f,		//31

			//seat cushion
			 0.7f, 0.1f,  0.0f,   0.9f, 0.9f, 0.8f, 	0.0f, 0.0f,		//32 //ATTEMPTED TO LEAVE  TEXTURE OFF CUSHION BUT STILL GOT texture O.0f
			-0.7f, 0.1f,  0.0f,   0.9f, 0.9f, 0.8f,		0.0f, 0.0f,		//33
			 0.7f, 0.2f,  0.0f,   0.9f, 0.9f, 0.8f, 	0.0f, 0.0f,		//34
			-0.7f, 0.2f,  0.0f,   0.9f, 0.9f, 0.8f,		0.0f, 0.0f,		//35

			 0.6f, 0.1f, -0.9f,   0.9f, 0.9f, 0.8f,     0.0f, 0.0f,		//36
			 0.6f, 0.2f, -0.9f,   0.9f, 0.9f, 0.8f, 	0.0f, 0.0f,		//37

			-0.6f, 0.1f, -0.9f,   0.9f, 0.9f, 0.8f, 	0.0f, 0.0f,		//38
			-0.6f, 0.2f, -0.9f,   0.9f, 0.9f, 0.8f, 	0.0f, 0.0f,		//39

			//chair base
			 0.6f, 0.0f,  0.0f,   0.17f, 0.11f, 0.05f, 	0.0f, 0.0f,		//40
			-0.6f, 0.0f,  0.0f,   0.17f, 0.11f, 0.05f,	1.0f, 0.0f,		//41
			 0.6f, 0.1f,  0.0f,   0.17f, 0.11f, 0.05f, 	0.25f, 0.0f,	//42
			-0.6f, 0.1f,  0.0f,   0.17f, 0.11f, 0.05f,	1.0f, 0.25f,	//43

			 0.6f, 0.0f, -1.0f,   0.17f, 0.11f, 0.05f,  0.0f, 0.25f,  	//44
			 0.6f, 0.1f, -1.0f,   0.17f, 0.11f, 0.05f, 	0.0f, 0.25f,	//45

		    -0.6f, 0.0f, -1.0f,   0.17f, 0.11f, 0.05f, 	1.0f, 0.25f,	//46
			-0.6f, 0.1f, -1.0f,   0.17f, 0.11f, 0.05f, 	1.0f, 0.25f,	//47


			//chair back tower 1
			0.5f, 0.1f, -0.9f,  0.17f, 0.11f, 0.05f, 	1.0f, 0.0f,		//48
			0.6f, 0.1f, -0.9f,  0.17f, 0.11f, 0.05f, 	0.75f,0.0f,		//49
			0.5f, 0.1f, -1.0f,  0.17f, 0.11f, 0.05f, 	0.0f, 0.0f,		//50
			0.6f, 0.1f, -1.0f,  0.17f, 0.11f, 0.05f, 	0.5f, 0.0f,		//51

			0.5f, 1.2f, -1.1f,  0.17f, 0.11f, 0.05f,	1.0f, 1.0f,		//52
			0.6f, 1.2f, -1.1f,  0.17f, 0.11f, 0.05f,	0.75f,1.0f,		//53
			0.5f, 1.2f, -1.2f,  0.17f, 0.11f, 0.05f,	0.0f, 1.0f,		//54
			0.6f, 1.2f, -1.2f,  0.17f, 0.11f, 0.05f,	0.5f, 1.0f,		//55

			//chair back tower 2
		   -0.5f, 0.1f, -0.9f,  0.17f, 0.11f, 0.05f, 	1.0f, 0.0f,		//56
		   -0.6f, 0.1f, -0.9f,  0.17f, 0.11f, 0.05f,	0.75f,0.0f,		//57
		   -0.5f, 0.1f, -1.0f,  0.17f, 0.11f, 0.05f,	0.0f, 0.0f,		//58
		   -0.6f, 0.1f, -1.0f,  0.17f, 0.11f, 0.05f, 	0.5f, 0.0f,		//59

		   -0.5f, 1.2f, -1.1f,  0.17f, 0.11f, 0.05f, 	1.0f, 1.0f,		//60
		   -0.6f, 1.2f, -1.1f,  0.17f, 0.11f, 0.05f,	0.75f, 1.0f,	//61
		   -0.5f, 1.2f, -1.2f,  0.17f, 0.11f, 0.05f,	0.0f, 1.0f,		//62
		   -0.6f, 1.2f, -1.2f,  0.17f, 0.11f, 0.05f,	0.5f, 1.0f,		//63

		   //chair horiz back top
		    0.5f, 1.1f, -1.1f,  0.39f, 0.26f, 0.12f,	0.0f, 0.0f,		//64
		   -0.5f, 1.1f, -1.1f,  0.39f, 0.26f, 0.12f,	1.0f, 0.0f,		//65
		   -0.5f, 1.2f, -1.1f,  0.39f, 0.26f, 0.12f,	1.0f, 0.5f,		//66
		    0.5f, 1.2f, -1.1f,  0.39f, 0.26f, 0.12f,	0.0f, 0.5f,		//67

			0.5f, 1.1f, -1.2f,  0.39f, 0.26f, 0.12f, 	0.0f, 0.0f,		//68
		   -0.5f, 1.1f, -1.2f,  0.39f, 0.26f, 0.12f,	1.0f, 0.0f,		//69
		   -0.5f, 1.2f, -1.2f,  0.39f, 0.26f, 0.12f,   	1.0f, 0.5f,		//70
		    0.5f, 1.2f, -1.2f,  0.39f, 0.26f, 0.12f, 	0.0f, 0.5f,		//71

			//chair horiz back bottom
			0.5f, 0.4f, -1.0f,  0.39f, 0.26f, 0.12f,	0.0f, 0.0f,		//72
		   -0.5f, 0.4f, -1.0f,  0.39f, 0.26f, 0.12f,	1.0f, 0.0f,		//73
		   -0.5f, 0.5f, -1.0f,  0.39f, 0.26f, 0.12f,	1.0f, 0.5f,		//74
			0.5f, 0.5f, -1.0f,  0.39f, 0.26f, 0.12f, 	0.0f, 0.5f,		//75

			0.5f, 0.4f, -1.05f,  0.39f, 0.26f, 0.12f, 	0.0f, 0.0f,		//76
		   -0.5f, 0.5f, -1.05f,  0.39f, 0.26f, 0.12f,	1.0f, 0.0f,		//77
		   -0.5f, 0.4f, -1.05f,  0.39f, 0.26f, 0.12f,  	1.0f, 0.5f,		//78
		    0.5f, 0.5f, -1.05f,  0.39f, 0.26f, 0.12f, 	0.0f, 0.5f,		//79

			//chair rung 1 front to back

			0.58f, -0.4f, -0.10f, 0.39f, 0.26f, 0.12f,	0.0f, 0.25f,	//80
			0.55f, -0.4f, -0.10f, 0.39f, 0.26f, 0.12f,	0.0f, 0.5f,		//81
			0.55f, -0.5f, -0.10f, 0.39f, 0.26f, 0.12f,	0.0f, 0.75f,	//82
			0.58f, -0.5f, -0.10f, 0.39f, 0.26f, 0.12f,	0.0f, 0.0f,		//83

			0.58f, -0.4f, -0.90f, 0.39f, 0.26f, 0.12f,	1.0f, 0.25f,	//84
			0.55f, -0.4f, -0.90f, 0.39f, 0.26f, 0.12f,	1.0f, 0.5f,		//85
			0.55f, -0.5f, -0.90f, 0.39f, 0.26f, 0.12f,	1.0f, 0.75f,	//86
			0.58f, -0.5f, -0.90f, 0.39f, 0.26f, 0.12f,	1.0f, 0.0f,		//87


			//chair rung 2 front to back

		   -0.58f, -0.4f, -0.10f, 0.39f, 0.26f, 0.12f,	0.0f, 0.25f,	//88
		   -0.55f, -0.4f, -0.10f, 0.39f, 0.26f, 0.12f,	0.0f, 0.5f,	 	//89
		   -0.55f, -0.5f, -0.10f, 0.39f, 0.26f, 0.12f,	0.0f, 0.75f,	//90
		   -0.58f, -0.5f, -0.10f, 0.39f, 0.26f, 0.12f,	0.0f, 0.0f,		//91

		   -0.58f, -0.4f, -0.90f, 0.39f, 0.26f, 0.12f,	1.0f, 0.25f,	//92
		   -0.55f, -0.4f, -0.90f, 0.39f, 0.26f, 0.12f,	1.0f, 0.5f,		//93
		   -0.55f, -0.5f, -0.90f, 0.39f, 0.26f, 0.12f,	1.0f, 0.75f,	//94
		   -0.58f, -0.5f, -0.90f, 0.39f, 0.26f, 0.12f,	1.0f, 0.0f,		//95

		   //chair rung across the front

		   -0.5f, -0.5f, -0.03f,  0.39f, 0.26f, 0.12f, 	0.0f, 0.0f,	//96
		    0.5f, -0.5f, -0.03f,  0.39f, 0.26f, 0.12f,	1.0f, 0.0f,	//97
		   -0.5f, -0.4f, -0.03f,  0.39f, 0.26f, 0.12f,	1.0f, 0.5f,	//98
		    0.5f, -0.4f, -0.03f,  0.39f, 0.26f, 0.12f,	0.0f, 0.5f,	//99


		   -0.5f, -0.5f, -0.06f,  0.39f, 0.26f, 0.12f, 	0.0f, 0.0f,	//100
			0.5f, -0.5f, -0.06f,  0.39f, 0.26f, 0.12f,	1.0f, 0.0f,	//101
		   -0.5f, -0.4f, -0.06f,  0.39f, 0.26f, 0.12f, 	1.0f, 0.5f,	//102
			0.5f, -0.4f, -0.06f,  0.39f, 0.26f, 0.12f,	0.0f, 0.5f,	//103

			//chair rung across the back

		   -0.5f, -0.5f, -0.85f,  0.39f, 0.26f, 0.12f, 	0.0f, 0.0f,	//104
		    0.5f, -0.5f, -0.85f,  0.39f, 0.26f, 0.12f,	1.0f, 0.0f,	//105
		   -0.5f, -0.4f, -0.85f,  0.39f, 0.26f, 0.12f, 	1.0f, 0.5f,	//106
		    0.5f, -0.4f, -0.85f,  0.39f, 0.26f, 0.12f,	0.0f, 0.5f,	//107


		   -0.5f, -0.5f, -0.88f,  0.39f, 0.26f, 0.12f, 	0.0f, 0.0f,	//108
			0.5f, -0.5f, -0.88f,  0.39f, 0.26f, 0.12f, 	1.0f, 0.0f,	//109
		   -0.5f, -0.4f, -0.88f,  0.39f, 0.26f, 0.12f,	1.0f, 0.5f,	//110
			0.5f, -0.4f, -0.88f,  0.39f, 0.26f, 0.12f, 	0.0f, 0.5f,	//111

	};

	//Index data to share position data
	GLuint indices[] = {

			//chair leg
			0, 1, 2, 	//Triangle 1
			1, 3, 2, 	//Triangle 2
			4, 5, 7, 	//Triangle 2
			4, 6, 7, 	//Triangle 4
			4, 0, 2, 	//Triangle 5
			4, 6, 2, 	//Triangle 6
			1, 6, 3, 	//Triangle 7
			2, 3, 6, 	//Triangle 8
			5, 1, 3, 	//Triangle 9
			5, 3, 7, 	//Triangle 10
			5, 4, 0, 	//Triangle 11
			5, 1, 0, 	//Triangle 12

			//chair leg 2

			 8, 9,10, 	//Triangle 13
			 9,11,10, 	//Triangle 14
			12,13,15, 	//Triangle 15
			12,14,15,	//Triangle 16
			12, 8,10,	//Triangle 17
			12,14,10,	//Triangle 18
			 9,14,11,	//Triangle 19
			10,11,14, 	//Triangle 20
			13, 9,11,	//Triangle 21
			13,11,15,	//Triangle 22
			13,12,8,	//Triangle 23
			13,9,8,		//Triangle 24

			//chair leg 3
			16,17,18,	//Triangle 25
			17,19,18,	//Triangle 26
			20,21,23,	//Triangle 27
			20,22,23,	//Triangle 28
			20,16,18,	//Triangle 29
			20,22,18,	//Triangle 30
			18,19,23,	//Triangle 31
			18,19,22,	//Triangle 32
			21,17,19,	//Triangle 33
			21,19,23,	//Triangle 34
			21,20,16,	//Triangle 35
			21,17,16,	//Triangle 36


			//chair leg 4
			24,25,29, 	//Triangle 37
			24,28,29,	//Triangle 38
			25,27,29,	//Triangle 39
			27,31,29,	//Triangle 40
			27,26,31,	//Triangle 41
			26,30,31,	//Triangle 42
			26,30,24, 	//Triangle 43
			24,28,30,	//Triangle 44
			29,28,30,	//Triangle 45
			29,31,30,	//Triangle 46
			24,25,26,	//Triangle 47
			25,27,26,	//Triangle 48


			//seat
			32,33,34,	//Triangle 49
			32,33,35,	//Triangle 50
			32,34,36,	//Triangle 51
			32,34,37,	//Triangle 52
			33,35,38,	//Triangle 53
			33,38,39,	//Triangle 54
			36,37,38,	//Triangle 55
			36,39,37,	//Triangle 56
			32,33,37,	//Triangle 57
			32,33,38,	//Triangle 58
			34,35,39,	//Triangle 59
			34,36,39,	//Triangle 60


			//BASE OF CHAIR
			40,41,42, 	//Triangle 61
			40,41,43,	//Triangle 62
			40,42,44,	//Triangle 63
			40,42,45,	//Triangle 64
			41,43,46,	//Triangle 65
			41,46,47,	//Triangle 66
			44,45,46,	//Triangle 67
			44,47,45,	//Triangle 68
			40,41,45,	//Triangle 69
			40,41,46,	//Triangle 70
			42,43,47,	//Triangle 71
			42,44,47,	//Triangle 72

			//chair tower 1
			48,49,53,	//Triangle 73
			48,52,53,	//Triangle 74
			49,51,55,	//Triangle 75
			49,53,55,	//Triangle 76
			51,50,55,	//Triangle 77
			50,54,55,	//Triangle 78
			50,54,52,	//Triangle 79
			50,48,52,	//Triangle 80
			52,53,54,	//Triangle 81
			53,55,54,	//Triangle 82
			48,49,50,	//Triangle 83
			49,51,50,	//Triangle 84

			//chair tower 2
			56,57,61,	//Triangle 85
			56,60,61,	//Triangle 86
			57,59,63,	//Triangle 87
			57,61,63,	//Triangle 88
			59,58,63,	//Triangle 89
			58,62,63,	//Triangle 90
			58,62,60,	//Triangle 91
			58,56,60,	//Triangle 92
			60,61,62,	//Triangle 93
			61,63,62,	//Triangle 94
			56,57,58,	//Triangle 95
			57,59,58,	//Triangle 96

			//chair horiz back
			64,65,67,	//Triangle 97
			64,65,66,	//Triangle 98
			64,68,71,	//Triangle 99
			64,67,71,	//Triangle 100
			68,69,70,	//Triangle 101
			68,71,70,	//Triangle 102
			65,69,64,	//Triangle 103
			66,70,69,	//Triangle 104
			66,70,67,	//Triangle 105
			67,71,70,	//Triangle 106
			65,64,68,	//Triangle 107
			65,69,64,	//Triangle 108

			//chair horiz back lower
			72,73,75,	//Triangle 109
			73,74,75,	//Triangle 110
			72,76,79,	//Triangle 111
			79,75,72,	//Triangle 112
			79,76,77,	//Triangle 113
			79,78,77,	//Triangle 114
			74,78,77,	//Triangle 115
			73,74,77,	//Triangle 116
			75,79,78,	//Triangle 117
			75,74,78,	//Triangle 118
			72,73,77,	//Triangle 119
			72,76,77,	//Triangle 120

			//chair rung front to back
			81,80,83,	//Triangle 121
			81,82,83,	//Triangle 122
			80,83,87,	//Triangle 123
			80,84,87,	//Triangle 124
			84,87,86,	//Triangle 125
			84,85,86,	//Triangle 126
			86,85,81,	//Triangle 127
			81,82,86,	//Triangle 128
			82,83,87,	//Triangle 129
			83,82,86,	//Triangle 130
			81,80,84,	//Triangle 131
			80,81,85,	//Triangle 132

			//chair rung 2 front to back
			89,88,91,	//Triangle 133
			89,90,91,	//Triangle 134
			88,91,95,	//Triangle 135
			91,95,92,	//Triangle 136
			92,95,94,	//Triangle 137
			92,93,94,	//Triangle 138
			94,93,89,	//Triangle 139
			89,90,94,	//Triangle 140
			91,90,95,	//Triangle 141
			90,94,95,	//Triangle 142
			89,88,92,	//Triangle 143
			89,93,92,	//Triangle 144

			//chair rung 3 across the front
			99,97,96,		//Triangle 145
			96,98,99,		//Triangle 146
			97,101,103,		//Triangle 147
			97,99,103,		//Triangle 148
			101,100,102,	//Triangle 149
			101,103,102,	//Triangle 150
			102,98,96,		//Triangle 151
			102,100,96,		//Triangle 152
			102,103,99,		//Triangle 153
			99,98,102,		//Triangle 154
			96,97,101,		//Triangle 155
			97,96,100,		//Triangle 156

			//chair rung 4 across the back
			104,105,107,	//Triangle 157
			107,106,104,	//Triangle 158
			105,109,111,	//Triangle 159
			105,107,111,	//Triangle 160
			109,111,110,	//Triangle 161
			109,108,110,	//Triangle 162
			106,110,108,	//Triangle 163
			106,104,108,	//Triangle 164
			106,110,111,	//Triangle 165
			106,107,111,	//Triangle 166
			104,105,109,	//Triangle 167
			104,108,109,	//Triangle 168



	};

	//Generate buffer IDs
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO); // activate vertex array object before binding and setting any VBOs and Vertex Attribute Pointers

	//Activate VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copy vertices to VBO

	//Activate EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // copy vertices to EBO

	//set attribute pointer 0 to hold position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0); //Enables vertex attribute

	//Set attribute pointer 1 to hold Normal data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Set attribute pointer 2 to hold Texture coordinate data
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0); // deactivates the VAO - good practice

}


//IMPLMEENTS UMOUSEMOVE
void UMouseMove(int x, int y)
{
	//immediately replaces center locked coordinates with new mouse coordinates
	if(checkMotion)
	{
		lastMouseX = x;
		lastMouseY = y;
		checkMotion = false;
	}


	//Gets the direction the mouse was moved in x and y
	mouseXOffset = x - lastMouseX;
	mouseYOffset = lastMouseY - y; //Inverted Y

	//Applies sensitivity to mouse direction
	mouseXOffset *= sensitivity;
	mouseYOffset *= sensitivity;

	//accumulates yaw and pitch variables
	yaw += mouseXOffset;
	pitch += mouseYOffset;

	//orbits around the center
	front.x = 10.0f * cos(yaw);
	front.y = 10.0f * sin(pitch);
	front.z = sin(yaw) * cos(pitch) * 10.0f;

	//maintains a 90 degree pitch and gimbal lock
	if(pitch > 89.0f)
		pitch = 89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;

	//if alt is down and mouse button down determine function of rotate or zoom
	if(altDown == true) {
		//Accumulates the yaw and pitch variables
		if(leftMouseButton == true){
			// rotate around the model
			cameraRotateAmt.x += mouseXOffset;
			cameraRotateAmt.y += mouseYOffset;
		}

		if(rightMouseButton == true && lastMouseY >y){
		//zoom in
				scale_by_y += 0.1f;
				scale_by_x += 0.1f;
				scale_by_z += 0.1f;


				//redisplay
				glutPostRedisplay();



		//else zoom out
			}else{
				scale_by_y -= 0.1f;
				scale_by_x -= 0.1f;
				scale_by_z -= 0.1f;

				//control zoom in size
				if(scale_by_y < 0.2f){
					scale_by_y = 0.2f;
					scale_by_x = 0.2f;
					scale_by_z = 0.2f;
				}

				//redisplay
				glutPostRedisplay();


			}
		//updates x and y
		lastMouseX = x;
		lastMouseY = y;

		}

	}






//IMPLEMENTS UMOUSECLICK FUNCTION
void UMouseClick(int button, int state, int x, int y)
{
	//sets the state for the alt key; true or false

	keymod = glutGetModifiers();

	// if keymod is alt down then altdown becomes true when released becomes false
	if ((keymod == GLUT_ACTIVE_ALT)&& (state == GLUT_DOWN)){
		altDown = true;
	}else{
		altDown = false;
	}


	//sets state for left mouse button  down
	if((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)){
		leftMouseButton = true;
	}

	//sets state for left mouse button up
	if((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP)){
		leftMouseButton = false;
	}

	// sets states for right mouse button down
	if((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN)){
		rightMouseButton = true;

	}
	//sets state for right mouse button down
	if((button == GLUT_RIGHT_BUTTON) && (state == GLUT_UP)){
		rightMouseButton = false;
	}

}


//IMPLEMENTS THE UKEYBOARD FUNCTION
void UKeyboard(unsigned char key, GLint x, GLint y)
{

switch(key){

	//registers p press to change state of vstate
	case 'p':
		cout<<"You pressed p, perspective display enabled."<<endl;
		vstate = 1;
		break;

	//registeres o press to change state of vstate
	case 'o':
		cout<<"You pressed o, orthographic display enabled."<<endl;
		vstate = 0;
		break;

	default: // Will display message for any other standard key pressed
		cout<<"Press p for perspective display, press o for orthographic display. "<<endl;
	}

}

//IMPLEMENTS THE U KEY RELEASED FUNCTION
void UKeyReleased(unsigned char key, GLint x, GLint y)
{
	cout<< "Key released"<<endl;  //simple display showing the user a key has been released.
	currentKey = '0';

}


//GENERATE AND LOAD TEXURE ... Note to self look into texture arrays
void UGenerateTexture(){
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height;

	unsigned char* image = SOIL_load_image("darkwood.jpg", &width, &height, 0, SOIL_LOAD_RGB);//loads texture file

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); //Unbind the texture
}










