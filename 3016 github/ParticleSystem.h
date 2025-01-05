#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <learnopengl/shader.h>

#include "random.h"

class ParticleSystem
{
public:
	ParticleSystem() : angle(0.0f), drawBuf(1), time(0), deltaT(0), nParticles(4000),
					   particleLifetime(2.0f), emitterPos(1, 0, 0), emitterDir(0, 1, 0) {}

	void initScene();
	void update(float t);
	void render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);

private:
	Random rand;

	// Position and direction of particle emitter
	glm::vec3 emitterPos, emitterDir;

	// Particle buffers
	GLuint posBuf[2], velBuf[2], age[2];
	// Particle VAOs
	GLuint particleArray[2];
	// Transform feedbacks
	GLuint feedback[2];

	GLuint drawBuf;

	int nParticles;
	float particleLifetime;
	float angle;
	float time, deltaT;

	GLuint bluewater = 0;

	GLuint randomTexture = 0;

	Shader particleShader;

	void initBuffers();
};