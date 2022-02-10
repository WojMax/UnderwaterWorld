#include "glew.h"
#include "freeglut.h"
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include <stdlib.h>  
#include <time.h>

#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "Camera.h"
#include "Texture.h"
#include "SOIL/stb_image_aug.h"
#include "ParticleEmitter.h"

GLuint programColor, programTexture, programSkybox, programParticle;

Core::Shader_Loader shaderLoader;

Core::RenderContext shipContext, sphereContext, cubeContext, terrainContext, fish1Context, coralContext, seaweedContext, whaleContext,plantContext, plant2Context, bubbleContext;

glm::vec3 cameraPos = glm::vec3(0, 0, 5);
glm::vec3 cameraDir; // camera forward vector
glm::vec3 cameraSide; // camera up vector
float cameraAngle = 0;

glm::mat4 cameraMatrix, perspectiveMatrix;

glm::vec3 lightDir = glm::vec3(1.0f, -0.9f, -1.0f);

glm::quat rotation = glm::quat(1, 0, 0, 0);

GLuint textureAsteroid, textureShip, textureTerrain, textureFish1, textureCoral, textureSeaweed, textureWhale, texturePlant, texturePlant2, textureBubble;

float delta_x = 0;
float delta_y = 0;
float delta_z = 0;
float old_x = 0;
float old_y = 0;
float old_z = 0;

std::vector<glm::vec3> keyPointsFish1({
	glm::vec3(200.0f, -2.0f, 1.0f),
	glm::vec3(195.0f, 0.0f, 1.0f),
	glm::vec3(190.0f, 0.0f, 1.0f),
	glm::vec3(185.0f, 0.0f, 1.0f),
	glm::vec3(180.0f, 0.0f, 1.0f),
	glm::vec3(175.0f, 0.0f, 1.0f),
	glm::vec3(170.0f, 0.0f, 1.0f),
	glm::vec3(165.0f, 0.0f, 1.0f),
	glm::vec3(160.0f, 0.0f, 1.0f),
	glm::vec3(155.0f, 0.0f, 1.0f),
	glm::vec3(150.0f, 0.0f, 1.0f),
	glm::vec3(145.0f, 0.0f, 1.0f),
	glm::vec3(140.0f, 0.0f, 1.0f),
	glm::vec3(135.0f, 0.0f, 1.0f),
	glm::vec3(130.0f, 0.0f, 1.0f),
	glm::vec3(124.0f, 0.0f, 1.0f),
	glm::vec3(120.0f, 0.0f, 5.0f),
	glm::vec3(115.0f, 1.0f, 5.0f),
	glm::vec3(110.0f, 1.0f, 5.0f),
	glm::vec3(105.0f, 1.2f, 7.0f),
	glm::vec3(95.0f, 1.5f, 15.0f),
	glm::vec3(85.0f, 1.5f, 15.0f),
	glm::vec3(75.0f, 2.0f, 15.0f),
	glm::vec3(65.0f, 2.0f, 15.0f),
	glm::vec3(55.0f, 3.0f, 10.0f),
	glm::vec3(45.0f, 3.0f, 9.0f),
	glm::vec3(35.0f, 4.0f, -2.0f),
	glm::vec3(25.0f, 3.0f, -10.0f),
	glm::vec3(15.0f, 3.0f, -20.0f),
	glm::vec3(5.0f, 3.0f, -30.0f),
	glm::vec3(-5.0f, 3.0f, -40.0f),
	glm::vec3(-15.0f, 3.0f, -50.0f),
	glm::vec3(-16.0f, 3.0f, -60.0f),
	glm::vec3(-18.0f, 3.0f, -70.0f),
	glm::vec3(-16.0f, 3.0f, -80.0f),
	glm::vec3(-10.0f, 3.0f, -90.0f),
	glm::vec3(-12.0f, 3.0f, -100.0f),
	glm::vec3(-13.0f, 3.0f, -110.0f),
	glm::vec3(-12.0f, 3.0f, -120.0f)
	});

std::vector<glm::quat> keyRotationFish1;


std::vector<glm::vec3> keyPointsFish2({
	glm::vec3(10.0f, -2.0f, -120.0f),
	glm::vec3(10.0f, 0.0f, -110.0f),
	glm::vec3(10.0f, 0.0f, -100.0f),
	glm::vec3(10.0f, 0.0f, -90.0f),
	glm::vec3(10.0f, 0.0f, -80.0f),
	glm::vec3(10.0f, 0.0f, -70.0f),
	glm::vec3(10.0f, 0.0f, -60.0f),
	glm::vec3(10.0f, 0.0f, -50.0f),
	glm::vec3(10.0f, 0.0f, -40.0f),
	glm::vec3(10.0f, 0.0f, -30.0f),
	glm::vec3(10.0f, 0.0f, -20.0f),
	glm::vec3(10.0f, 0.0f, -10.0f),
	glm::vec3(10.0f, 0.0f, 0.0f),
	glm::vec3(10.0f, 0.0f, 10.0f),
	glm::vec3(10.0f, 0.0f, 20.0f),
	glm::vec3(15.0f, 0.0f, 30.0f),
	glm::vec3(20.0f, 0.0f, 40.0f),
	glm::vec3(30.0f, 1.0f, 50.0f),
	glm::vec3(20.0f, 1.0f, 60.0f),
	glm::vec3(25.0f, 1.2f, 70.0f),
	glm::vec3(20.0f, 1.5f, 80.0f),
	glm::vec3(15.0f, 1.5f, 90.0f),
	glm::vec3(15.0f, 2.0f, 100.0f),
	glm::vec3(10.0f, 2.0f, 110.0f),
	glm::vec3(5.0f, 3.0f, 120.0f),
	glm::vec3(0.0f, 3.0f, 130.0f),
	glm::vec3(-10.0f, 4.0f, 140.0f),
	glm::vec3(-20.0f, 3.0f, 150.0f),
	glm::vec3(-30.0f, 3.0f, 160.0f),
	glm::vec3(-40.0f, 3.0f, 170.0f),
	glm::vec3(-50.0f, 3.0f, 180.0f),
	glm::vec3(-60.0f, 3.0f, 190.0f),
	glm::vec3(-70.0f, 3.0f, 200.0f),
	glm::vec3(-80.0f, 3.0f, 200.0f),
	glm::vec3(-90.0f, 3.0f, 200.0f),
	glm::vec3(-100.0f, 3.0f, 200.0f),
	glm::vec3(-110.0f, 3.0f, 200.0f),
	glm::vec3(-120.0f, 3.0f, 200.0f),
	glm::vec3(-130.0f, 3.0f, 200.0f)
	});

std::vector<glm::quat> keyRotationFish2;

float whaleX = 200;
float whaleY = 40;
float whaleZ = 0;

std::vector<glm::vec3> keyPointsWhale;
std::vector<glm::quat> keyRotationWhale;


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

//random x and z
const int CORALS_AMOUNT = 50;

float randX[CORALS_AMOUNT];
float randZ[CORALS_AMOUNT];

const int WEED_AMOUNT = 100;

float SWrandX[WEED_AMOUNT];
float SWrandZ[WEED_AMOUNT];

const int PLANT_AMOUNT = 100;

float PLrandX[PLANT_AMOUNT];
float PLrandZ[PLANT_AMOUNT];

const int PLANT2_AMOUNT = 100;

float PrandX[PLANT2_AMOUNT];
float PrandZ[PLANT2_AMOUNT];

const int BUBBLE_AMOUNT = 300;

float BrandX[BUBBLE_AMOUNT];
float BrandZ[BUBBLE_AMOUNT];
float BubbleY[BUBBLE_AMOUNT];
float Bspeed[BUBBLE_AMOUNT];

ParticleEmitter* ParticlesEmitter;

void setUpUniformsParticles(glm::mat4 transformation)
{
	GLuint program = programParticle;

	glUniformMatrix4fv(glGetUniformLocation(program, "M_v"), 1, GL_FALSE, (float*)&cameraMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "M_p"), 1, GL_FALSE, (float*)&perspectiveMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&transformation);
	glUniform1f(glGetUniformLocation(program, "particleSize"), 0.0075f);
}

void drawParticle(glm::mat4 transformation)
{
	GLuint program = programParticle;

	glUseProgram(program);

	setUpUniformsParticles(transformation);

	ParticlesEmitter->update(0.05f);
	ParticlesEmitter->draw();


	glUseProgram(0);
}

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
	switch (key)
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

void drawObjectColor(Core::RenderContext context, glm::mat4 modelMatrix, glm::vec3 color)
{
	GLuint program = programColor;

	glUseProgram(program);

	glUniform3f(glGetUniformLocation(program, "objectColor"), color.x, color.y, color.z);
	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawContext(context);

	glUseProgram(0);
}

void drawObjectTexture(Core::RenderContext context, glm::mat4 modelMatrix, GLuint textureId)
{
	GLuint program = programTexture;

	glUseProgram(program);

	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);
	Core::SetActiveTexture(textureId, "textureSampler", program, 0);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawContext(context);

	glUseProgram(0);
}

void drawSkybox(Core::RenderContext context, glm::mat4 modelMatrix, GLuint textureId)
{
	GLuint program = programSkybox;

	glUseProgram(program);
	cameraMatrix = glm::mat4(glm::mat3(cameraMatrix));

	glDepthFunc(GL_LEQUAL);
	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);
	Core::SetActiveTexture(textureId, "skybox", program, 0);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * glm::scale(glm::vec3(200.0f));

	glUniformMatrix4fv(glGetUniformLocation(program, "projectionView"), 1, GL_FALSE, (float*)&transformation);

	Core::DrawContext(context);

	glDepthFunc(GL_LESS);
	glUseProgram(0);
}

glm::mat4 animationMatrix(float time, std::vector<glm::vec3> keyPoints, std::vector<glm::quat> keyRotation) {
	float speed = 2.;
	time = time * speed;
	std::vector<float> distances;
	float timeStep = 0;
	for (int i = 0; i < keyPoints.size() - 1; i++) {
		timeStep += (keyPoints[i] - keyPoints[i + 1]).length();
		distances.push_back((keyPoints[i] - keyPoints[i + 1]).length());
	}
	time = fmod(time, timeStep);

	int index = 0;

	while (distances[index] <= time) {
		time = time - distances[index];
		index += 1;
	}

	float t = time / distances[index];

	int size = keyPoints.size() - 1;
	int rotationSize = keyRotation.size() - 1;

	glm::vec3 pos = glm::catmullRom(keyPoints[index - 1], keyPoints[index], keyPoints[index + 1], keyPoints[index + 2], t);

	auto a1 = keyRotation[index] * glm::exp(-(glm::log(glm::inverse(keyRotation[index]) * keyRotation[index - 1]) + glm::log(glm::inverse(keyRotation[index]) * keyRotation[index + 1])) * glm::quat(1 / 4, 1 / 4, 1 / 4, 1 / 4));
	auto a2 = keyRotation[index + 1] * glm::exp(-(glm::log(glm::inverse(keyRotation[index + 1]) * keyRotation[index]) + glm::log(glm::inverse(keyRotation[index + 1]) * keyRotation[index + 2])) * glm::quat(1 / 4, 1 / 4, 1 / 4, 1 / 4));
	auto animationRotation = glm::squad(keyRotation[index], keyRotation[index + 1], a1, a2, t);

	glm::mat4 result = glm::translate(pos) * glm::mat4_cast(animationRotation);

	return result;
}

float RandomNumber(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

void renderScene()
{
	float time = glutGet(GLUT_ELAPSED_TIME) / 1000.f;
	// Aktualizacja macierzy widoku i rzutowania
	cameraMatrix = createCameraMatrix();
	perspectiveMatrix = Core::createPerspectiveMatrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.1f, 0.3f, 1.0f);

	glm::mat4 shipInitialTransformation = glm::translate(glm::vec3(-0.1f, -0.4f, 0)) * glm::rotate(glm::radians(90.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(0.1f));
	glm::mat4 shipModelMatrix = glm::translate(cameraPos + cameraDir * 1.0f) * glm::mat4_cast(glm::inverse(rotation)) * shipInitialTransformation;


	drawObjectTexture(shipContext, shipModelMatrix, textureShip);

	//drawObjectTexture(whaleContext, glm::translate(glm::vec3(0, 0, 0)) * glm::scale(glm::vec3(20.0f)), textureWhale);

	drawObjectTexture(sphereContext, glm::translate(glm::vec3(0, 10, 0)) * glm::scale(glm::vec3(0.05f)), textureBubble);

	drawObjectTexture(terrainContext, glm::translate(glm::vec3(0, -8, 0)) * glm::rotate(glm::radians(90.0f), glm::vec3(-1, 0, 0)) * glm::scale(glm::vec3(1.0f)), textureTerrain);


	for (int i = 1; i <= CORALS_AMOUNT; i++)
	{
			drawObjectTexture(coralContext, glm::translate(glm::vec3(randX[i], -5.2, randZ[i])) * glm::scale(glm::vec3(0.05f)) * glm::rotate(glm::radians(90.0f), glm::vec3(-1, 0, 0)), textureCoral);
	}

	for (int i = 0; i <= WEED_AMOUNT; i++)
	{
		drawObjectTexture(seaweedContext, glm::translate(glm::vec3(SWrandX[i], -5.2, SWrandZ[i])) * glm::rotate(glm::radians(90.0f), glm::vec3(0, -1, 0)), textureSeaweed);

	}

	for (int i = 0; i <= PLANT_AMOUNT; i++)
	{
		drawObjectTexture(plantContext, glm::translate(glm::vec3(PLrandX[i], -5.2, PLrandZ[i]))  * glm::rotate(glm::radians(90.0f), glm::vec3(0, -1, 0)), texturePlant);

	}
	
	for (int i = 0; i <= PLANT2_AMOUNT; i++)
	{
		drawObjectTexture(plant2Context, glm::translate(glm::vec3(PrandX[i], -5.2, PrandZ[i])) * glm::scale(glm::vec3(0.1f)) * glm::rotate(glm::radians(90.0f), glm::vec3(0, -1, 0)), texturePlant2);

	}

	for (int i = 0; i <= BUBBLE_AMOUNT; i++)
	{
		if (BubbleY[i] >= 20.0)
		{
			BubbleY[i] = RandomNumber(-7, -5);
		}
		drawObjectTexture(sphereContext, glm::translate(glm::vec3(BrandX[i], BubbleY[i] += Bspeed[i], BrandZ[i])) * glm::scale(glm::vec3(0.05f)), textureBubble);
	}

	glm::vec3  change1 = glm::vec3(0, 3, 0);
	glm::vec3  change2 = glm::vec3(0, 0, 0);
	glm::vec3  change3 = glm::vec3(3, 0, 0);
	glm::vec3  change4 = glm::vec3(0, 2, 1);
	float time2 = time;
	for (int i = 0; i < 30; i++) {
		if (time > -10) {
			glm::mat4 fish1Transformation = glm::scale(glm::vec3(0.3f));
			glm::mat4 matrix = animationMatrix(time + 15, keyPointsFish1, keyRotationFish1);
			drawObjectTexture(fish1Context, matrix * glm::translate(change2) * fish1Transformation, textureFish1);
			drawObjectTexture(fish1Context, matrix * glm::translate(change3) * fish1Transformation, textureFish1);
			drawObjectTexture(fish1Context, matrix * glm::translate(change4) * fish1Transformation, textureFish1);

			glm::mat4 matrix2 = animationMatrix(time + 15, keyPointsFish2, keyRotationFish2);
			drawObjectTexture(fish1Context, matrix2 * glm::translate(change2) * fish1Transformation, textureFish1);
			drawObjectTexture(fish1Context, matrix2 * glm::translate(change3) * fish1Transformation, textureFish1);
			drawObjectTexture(fish1Context, matrix2 * glm::translate(change4) * fish1Transformation, textureFish1);

			time -= 3;
		}

		if (time2 > -10)
		{
			glm::mat4 matrix3 = animationMatrix(time2 + 15, keyPointsWhale, keyRotationWhale);
			drawObjectTexture(whaleContext, matrix3 * glm::scale(glm::vec3(150.0f)), textureWhale);
		}
	}


	drawParticle(shipModelMatrix * glm::translate(glm::vec3(-2.0f, -3.2f, 0.8f)) * glm::rotate(glm::radians(-90.0f), glm::vec3(0, -1, 0)) );

	drawSkybox(cubeContext, glm::translate(glm::vec3(0, 0, 0)), cubemapTexture);

	glutSwapBuffers();
}

void loadModelToContext(std::string path, Core::RenderContext& context)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	context.initFromAssimpMesh(scene->mMeshes[0]);
}

void initKeyRotation(std::vector<glm::vec3>& keyPoints, std::vector<glm::quat>& keyRotation) {
	glm::vec3 oldDirection = glm::vec3(0, 0, 1);
	glm::quat oldRotationCamera = glm::quat(1, 0, 0, 0);
	for (int i = 0; i < keyPoints.size() - 1; i++) {
		glm::vec3 newDirection = glm::normalize(keyPoints[i + 1] - keyPoints[i]);
		glm::quat rotation = glm::normalize(glm::rotationCamera(oldDirection, newDirection) * oldRotationCamera);
		keyRotation.push_back(rotation);
		oldDirection = newDirection;
		oldRotationCamera = rotation;
	}
	keyRotation.push_back(glm::quat(1, 0, 0, 0));
}

void init()
{
	for (int i = 0; i < CORALS_AMOUNT; i++)
	{
		if (rand() % 2 == 0)
		{
			randX[i] = rand() % 50;
		}
		else
		{
			randX[i] = -(rand() % 50);
		}
		if (rand() % 2 == 0)
		{
			randZ[i] = rand() % 50;
		}
		else
		{
			randZ[i] = -(rand() % 50);
		}
	}
	//seaweed
	for (int i = 0; i < WEED_AMOUNT; i++)
	{
		if (rand() % 2 == 0)
		{
			SWrandX[i] = rand() % 50;
		}
		else
		{
			SWrandX[i] = -(rand() % 50);
		}
		if (rand() % 2 == 0)
		{
			SWrandZ[i] = rand() % 50;
		}
		else
		{
			SWrandZ[i] = -(rand() % 50);
		}
	}
	//plant1
	for (int i = 0; i < PLANT_AMOUNT; i++)
	{
		if (rand() % 2 == 0)
		{
			PLrandX[i] = rand() % 50;
		}
		else
		{
			PLrandX[i] = -(rand() % 50);
		}
		if (rand() % 2 == 0)
		{
			PLrandZ[i] = rand() % 50;
		}
		else
		{
			PLrandZ[i] = -(rand() % 50);
		}
	}	
	//plant2
	for (int i = 0; i < PLANT2_AMOUNT; i++)
	{
		if (rand() % 2 == 0)
		{
			PrandX[i] = rand() % 50;
		}
		else
		{
			PrandX[i] = -(rand() % 50);
		}
		if (rand() % 2 == 0)
		{
			PrandZ[i] = rand() % 50;
		}
		else
		{
			PrandZ[i] = -(rand() % 50);
		}
	}
	//Bubbles
	for (int i = 0; i < BUBBLE_AMOUNT; i++)
	{
		if (rand() % 2 == 0)
		{
			BrandX[i] = rand() % 50;
		}
		else
		{
			BrandX[i] = -(rand() % 50);
		}
		if (rand() % 2 == 0)
		{
			BrandZ[i] = rand() % 50;
		}
		else
		{
			BrandZ[i] = -(rand() % 50);
		}
		BubbleY[i] = RandomNumber(-7, -5);
		Bspeed[i] = RandomNumber(0.05, 0.1);
	}

	srand(time(0));
	glEnable(GL_DEPTH_TEST);
	ParticlesEmitter = new ParticleEmitter();

	programColor = shaderLoader.CreateProgram("shaders/shader_color.vert", "shaders/shader_color.frag");
	programTexture = shaderLoader.CreateProgram("shaders/shader_tex.vert", "shaders/shader_tex.frag");
	programSkybox = shaderLoader.CreateProgram("shaders/shader_skybox.vert", "shaders/shader_skybox.frag");
	programParticle = shaderLoader.CreateProgram("shaders/shader_particle.vert", "shaders/shader_particle.frag");

	loadModelToContext("models/submarine.obj", shipContext);
	loadModelToContext("models/cube.obj", cubeContext);
	loadModelToContext("models/sphere.obj", sphereContext);
	loadModelToContext("models/desert_terrain.obj", terrainContext);
	loadModelToContext("models/fish/fish2.obj", fish1Context);
	loadModelToContext("models/coral.obj", coralContext);
	loadModelToContext("models/seaweed.obj", seaweedContext);
	loadModelToContext("models/fish/whale.obj", whaleContext);
	loadModelToContext("models/plant.obj", plantContext);
	loadModelToContext("models/plant2.obj", plant2Context);
	loadModelToContext("models/bubble.obj", bubbleContext);

	textureAsteroid = Core::LoadTexture("textures/a.jpg");
	textureShip = Core::LoadTexture("textures/submarine1.png");
	cubemapTexture = loadCubemap();
	textureTerrain = Core::LoadTexture("textures/diffuse.png");
	textureFish1 = Core::LoadTexture("textures/fish/fish2.png");
	textureCoral = Core::LoadTexture("textures/coral.jpg");
	textureSeaweed = Core::LoadTexture("textures/seaweed.png");
	textureWhale = Core::LoadTexture("textures/fish/whale.jpg");
	texturePlant = Core::LoadTexture("textures/plant.png");
	texturePlant2 = Core::LoadTexture("textures/plant2.jpg");
	textureBubble = Core::LoadTexture("textures/bubble.png");

	for (int i = 0; i <= 140; i++)
	{
		whaleX -= 5;
		keyPointsWhale.push_back(glm::vec3(whaleX, whaleY, whaleZ));
	}


	initKeyRotation(keyPointsFish1, keyRotationFish1);
	initKeyRotation(keyPointsFish2, keyRotationFish2);
	initKeyRotation(keyPointsWhale, keyRotationWhale);
}

void shutdown()
{
	shaderLoader.DeleteProgram(programColor);
	shaderLoader.DeleteProgram(programTexture);
	shaderLoader.DeleteProgram(programParticle);
	if (NULL != ParticlesEmitter) delete ParticlesEmitter;
}

void idle()
{
	glutPostRedisplay();
}

int main(int argc, char** argv)
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
