#pragma once
#ifndef _BALL_H_
#define _BALL_H_

#include "core.h"
#include "cube.h"
#include "Sphere.h"
#include "Box.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////

class Ball
{
private:
	//parameters
	glm::vec3 position, velocity, force, 
		gravForce, gravForceMult, norm, windVelocity;
	GLfloat mass, radius, elastic, 
		friction, airResist, gravMult, grav;
	
	Cube* cube;
	bool lastHit;
	Plain* lastHitPlain;
	Box* box;
	std::vector<std::pair<int, int>> edgeRecord;


public:
	Ball(Cube * cube);
	~Ball();

	void update(GLfloat timeStep, GLfloat& restTime);
	void draw(const glm::mat4& viewProjMtx, GLuint shader);
	void ApplyForce(glm::vec3 f);
	
	glm::vec3 GetPos();
	glm::vec3 GetVelocity();
	GLfloat GetRadius() { return this->radius; }

	void addNorm(glm::vec3 normal);
	void clearNorm();

	void normalize();
	void translate(float dist);

	glm::vec3 getNorm();
	glm::vec3 getPos();
	void boxCollision(GLfloat timeStep, GLfloat& restTime);
	void renderUpdate();

	void defaultForces();
	GLfloat* getMass();
	GLfloat* getGravMult();
	GLfloat* getAirResist();
	GLfloat getRadius();
	GLfloat* getFriction();
	GLfloat* getElastic();
	GLfloat* getPosX();
	GLfloat* getPosY();
	GLfloat* getPosZ();
	GLfloat* getWindX() { return &this->windVelocity.x; }
	GLfloat* getWindY() { return &this->windVelocity.y; }
	GLfloat* getWindZ() { return &this->windVelocity.z; }
	GLfloat* getVelX();
	GLfloat* getVelY();
	GLfloat* getVelZ();

	std::vector<std::pair<glm::vec3, glm::vec3>> getEgde();

	void restoreDefault();
	std::vector<Plain*> getCollider() { return this->box->getCollider(); }
};

////////////////////////////////////////////////////////////////////////////////

#endif
