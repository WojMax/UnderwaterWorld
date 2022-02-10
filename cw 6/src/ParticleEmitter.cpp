#include "ParticleEmitter.h"
#include <cstdlib>

ParticleEmitter::ParticleEmitter()
{
	vertexBuffer = 0;
	positionBuffer = 0;
	this->particles.resize(100);
	for (int i = 0; i < this->particles.size(); ++i)
	{
		this->particles[i].position = glm::vec3(randomFloat(-1.0f, 1.0f), randomFloat(-1.0f, 1.0f), randomFloat(-1.0f, 1.0f));
		this->particles[i].lifetime = randomFloat(1.0f, 2.0f);
	}

	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &positionBuffer);

	std::vector< float > vertices;
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);

	vertices.push_back(1.0f);
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);

	vertices.push_back(0.0f);
	vertices.push_back(1.0f);
	vertices.push_back(0.0f);

	vertices.push_back(1.0f);
	vertices.push_back(1.0f);
	vertices.push_back(0.0f);

	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, this->positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, this->particles.size() * 4 * sizeof(float), this->positions, GL_DYNAMIC_DRAW);
}

float ParticleEmitter::randomFloat(float min, float max)
{
	return  (max - min) * ((((float)rand()) / (float)RAND_MAX)) + min;
}

void ParticleEmitter::update(const float dt)
{
	for (int i = 0; i < this->particles.size(); ++i)
	{
		this->particles[i].lifetime -= dt;

		if (this->particles[i].lifetime <= 0.0f)
		{
			this->particles[i].position = glm::vec3(randomFloat(-1.0f, 1.0f), randomFloat(-1.0f, 1.0f), randomFloat(-1.0f, 1.0f));
			this->particles[i].lifetime = randomFloat(1.0f, 2.0f);
		}

		this->particles[i].position -= glm::vec3(0.0f, 0.0f, dt * 1.0f);

		this->positions[i * 4 + 0] = this->particles[i].position[0];
		this->positions[i * 4 + 1] = this->particles[i].position[1];
		this->positions[i * 4 + 2] = this->particles[i].position[2];
		this->positions[i * 4 + 3] = this->particles[i].lifetime;
	}
}

void ParticleEmitter::draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(4);

	glBindBuffer(GL_ARRAY_BUFFER, this->positionBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->particles.size() * 4 * sizeof(float), this->positions);

	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, this->positionBuffer);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
	glVertexAttribDivisor(4, 1);

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, this->particles.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(4);
}

ParticleEmitter::~ParticleEmitter()
{
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &positionBuffer);
}