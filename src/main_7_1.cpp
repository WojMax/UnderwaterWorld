#include "glew.h"
#include "freeglut.h"
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>
#include <vector>
//#include "SOIL/stb_image.h"



#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "Camera.h"
#include "Texture.h"
#include "SOIL/stb_image_aug.h"

float skyboxSize = 20.0f;


float skyboxVertices[] = {
	-skyboxSize,  skyboxSize, -skyboxSize,
	-skyboxSize, -skyboxSize, -skyboxSize,
	 skyboxSize, -skyboxSize, -skyboxSize,
	 skyboxSize, -skyboxSize, -skyboxSize,
	 skyboxSize,  skyboxSize, -skyboxSize,
	-skyboxSize,  skyboxSize, -skyboxSize,

	-skyboxSize, -skyboxSize,  skyboxSize,
	-skyboxSize, -skyboxSize, -skyboxSize,
	-skyboxSize,  skyboxSize, -skyboxSize,
	-skyboxSize,  skyboxSize, -skyboxSize,
	-skyboxSize,  skyboxSize,  skyboxSize,
	-skyboxSize, -skyboxSize,  skyboxSize,

	 skyboxSize, -skyboxSize, -skyboxSize,
	 skyboxSize, -skyboxSize,  skyboxSize,
	 skyboxSize,  skyboxSize,  skyboxSize,
	 skyboxSize,  skyboxSize,  skyboxSize,
	 skyboxSize,  skyboxSize, -skyboxSize,
	 skyboxSize, -skyboxSize, -skyboxSize,

	-skyboxSize, -skyboxSize,  skyboxSize,
	-skyboxSize,  skyboxSize,  skyboxSize,
	 skyboxSize,  skyboxSize,  skyboxSize,
	 skyboxSize,  skyboxSize,  skyboxSize,
	 skyboxSize, -skyboxSize,  skyboxSize,
	-skyboxSize, -skyboxSize,  skyboxSize,

	-skyboxSize,  skyboxSize, -skyboxSize,
	 skyboxSize,  skyboxSize, -skyboxSize,
	 skyboxSize,  skyboxSize,  skyboxSize,
	 skyboxSize,  skyboxSize,  skyboxSize,
	-skyboxSize,  skyboxSize,  skyboxSize,
	-skyboxSize,  skyboxSize, -skyboxSize,

	-skyboxSize, -skyboxSize, -skyboxSize,
	-skyboxSize, -skyboxSize,  skyboxSize,
	 skyboxSize, -skyboxSize, -skyboxSize,
	 skyboxSize, -skyboxSize, -skyboxSize,
	-skyboxSize, -skyboxSize,  skyboxSize,
	 skyboxSize, -skyboxSize,  skyboxSize
};

GLuint programColor;
GLuint programTexture;
GLuint programSkybox;
unsigned int loadCubemap(std::vector<std::string> faces);
unsigned int skyboxVAO, skyboxVBO;

Core::Shader_Loader shaderLoader;

obj::Model shipModel;
obj::Model sphereModel;
obj::Model cubeModel;

glm::vec3 cameraPos = glm::vec3(0, 0, 5);
glm::vec3 cameraDir; // camera forward vector
glm::vec3 cameraSide; // camera up vector
float cameraAngle = 0;

float delta_x = 0;
float delta_y = 0;
float delta_z = 0;
float old_x = 0;
float old_y = 0;
float old_z = 0;


glm::mat4 cameraMatrix, perspectiveMatrix;

glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, -0.9f, -1.0f));

glm::quat rotation = glm::quat(1, 0, 0, 0);
glm::vec3 randomPlanets[10];

GLuint textureAsteroid;
GLuint textureShip;

std::vector<std::string> faces
{
		"textures/skybox/right.jpg",
		"textures/skybox/left.jpg",
		"textures/skybox/top.jpg",
		"textures/skybox/bottom.jpg",
		"textures/skybox/front.jpg",
		"textures/skybox/back.jpg"
};

unsigned int cubemapTexture;

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap()
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < 6; i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << stbi_failure_reason() << std::endl;
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void keyboard(unsigned char key, int x, int y)
{
	
	float angleSpeed = 0.1f;
	float moveSpeed = 0.1f;
	switch(key)
	{
	case 'z': cameraAngle -= angleSpeed; break;
	case 'x': cameraAngle += angleSpeed; break;
	case 'w': cameraPos += cameraDir * moveSpeed; break;
	case 's': cameraPos -= cameraDir * moveSpeed; break;
	case 'd': cameraPos += cameraSide * moveSpeed; break;
	case 'a': cameraPos -= cameraSide * moveSpeed; break;
	}
}

void mouse(int x, int y)
{
	delta_x = old_x - x;
	delta_y = old_y - y;
	old_x = x;
	old_y = y;
}

glm::mat4 createCameraMatrix()
{

	glm::quat rotation_x = glm::angleAxis(delta_y * 0.01f, glm::vec3(1, 0, 0));
	delta_y = 0;
	glm::quat rotation_y = glm::angleAxis(delta_x * 0.01f, glm::vec3(0, 1, 0));
	delta_x = 0;
	glm::quat rotation_z = glm::angleAxis(delta_z * 0.01f, glm::vec3(0, 0, 1));
	delta_z = 0;

	glm::quat change = rotation_y * rotation_x * rotation_z;
	rotation = glm::normalize(change * rotation);

	glm::vec3 first = glm::vec3(0, 0, -1);
	glm::vec3 second = glm::vec3(1, 0, 0);
	cameraDir = glm::inverse(rotation) * first;
	cameraSide = glm::inverse(rotation) * second;

	return Core::createViewMatrixQuat(cameraPos, rotation);
}

void drawObjectColor(obj::Model * model, glm::mat4 modelMatrix, glm::vec3 color)
{
	GLuint program = programColor;

	glUseProgram(program);

	glUniform3f(glGetUniformLocation(program, "objectColor"), color.x, color.y, color.z);
	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glUseProgram(0);
}

void drawObjectTexture(obj::Model * model, glm::mat4 modelMatrix, GLuint textureId)
{
	GLuint program = programTexture;

	glUseProgram(program);

	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);
	Core::SetActiveTexture(textureId, "textureSampler", program, 0);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glUseProgram(0);
}
void renderScene()
{
	// Update of camera and perspective matrices
	cameraMatrix = createCameraMatrix();
	perspectiveMatrix = Core::createPerspectiveMatrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.1f, 0.3f, 1.0f);

	glm::mat4 shipInitialTransformation = glm::translate(glm::vec3(0, -0.4f, 0)) * glm::rotate(glm::radians(90.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(0.08f));
	glm::mat4 shipModelMatrix = glm::translate(cameraPos + cameraDir * 1.0f) * glm::mat4_cast(glm::inverse(rotation)) * shipInitialTransformation;
	//drawObjectColor(&shipModel, shipModelMatrix, glm::vec3(0.6f));
	drawObjectTexture(&shipModel, shipModelMatrix, textureShip);

	drawObjectTexture(&sphereModel, glm::translate(glm::vec3(0, 0, 0)), textureAsteroid);
	/*for (int i = 0; i < 10;i++)
	{
		drawObjectTexture(&sphereModel, glm::translate(randomPlanets[i]), textureAsteroid);
	}*/
	glm::mat4 cubeInitialTransformation = glm::scale(glm::vec3(100.0f));
	glUseProgram(programSkybox);
	cameraMatrix = glm::mat4(glm::mat3(cameraMatrix));
	// draw skybox as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	//Core::SetActiveTexture(cubemapTexture, "skybox", programSkybox, 0);
	glUniform1i(glGetUniformLocation(programSkybox, "skybox"), 0);
	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * cubeInitialTransformation;
	glUniformMatrix4fv(glGetUniformLocation(programSkybox, "projectionView"), 1, GL_FALSE, (float*)&transformation);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	Core::DrawModel(&cubeModel);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
	//glUseProgram(0);

	//drawObjectTexture(&sphereModel, glm::translate(glm::vec3(0,0,0)), textureAsteroid);
	//drawObjectTexture(&sphereModel, glm::translate(glm::vec3(0, 0, 0)), textureAsteroid);

	glutSwapBuffers();
}

void createSkybox() {
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void init()
{
	srand(time(0));
	glEnable(GL_DEPTH_TEST);
	programColor = shaderLoader.CreateProgram("shaders/shader_color.vert", "shaders/shader_color.frag");
	programTexture = shaderLoader.CreateProgram("shaders/shader_tex.vert", "shaders/shader_tex.frag");
	shipModel = obj::loadModelFromFile("models/submarine_low.obj");
	sphereModel = obj::loadModelFromFile("models/sphere.obj");
	cubeModel = obj::loadModelFromFile("models/cube.obj");
	textureAsteroid = Core::LoadTexture("textures/a.jpg");
	textureShip = Core::LoadTexture("textures/submarine.png");
	programSkybox = shaderLoader.CreateProgram("shaders/shader_skybox.vert", "shaders/shader_skybox.frag");

	/*for (int i = 0; i < 10;i++)
	{
		randomPlanets[i] = glm::ballRand(10.1f);
	}*/

	// skybox
	cubemapTexture = loadCubemap();
	//createSkybox();
}

void shutdown()
{
	shaderLoader.DeleteProgram(programColor);
	shaderLoader.DeleteProgram(programTexture);
}

void idle()
{
	glutPostRedisplay();
}

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(600, 600);
	glutCreateWindow("OpenGL Pierwszy Program");
	glewInit();

	init();
	glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(mouse);
	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);

	glutMainLoop();

	shutdown();

	return 0;
}
