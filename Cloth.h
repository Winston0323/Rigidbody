#pragma once
#pragma once
#ifndef _CLOTH_H_
#define _CLOTH_H_

#include "core.h"
#include "plain.h"
#include "Ball.h"
#include <iostream>
#include "glm/gtc/random.hpp"
#include <glm/gtc/type_ptr.hpp>

////////////////////////////////////////////////////////////////////////////////

class Cloth
{
private:
	//cloth properties
	glm::vec3 initPos;
	glm::vec3 color;
	GLfloat springConst;
	GLfloat dampingConst;

	//Opengl varibles
	GLuint VAO;
	GLuint VBO_positions, VBO_normals, EBO;
	glm::mat4 model;

	//render properties
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
	
	//particles properties
	std::vector<glm::vec3> velocitys;
	std::vector<glm::vec3> forces;
	std::vector<bool> isFixed;
	GLfloat elastic;
	GLfloat mass;
	GLfloat radius;

	int pNum;

	bool isEular;
	glm::vec3 force;

	//environment properties
	GLfloat dragCo;
	GLfloat liftCo;
	GLfloat grav;
	GLfloat gravMult;
	GLfloat airResist;
	GLfloat friction;
	glm::vec3 gravForce;
	glm::vec3 windVelocity;

	//springs
	std::vector<std::tuple<int, int, GLfloat>> springs;
	std::vector <std::vector<int>> springRecords;
	std::vector<std::tuple<int, int, int>> triangles;
	std::vector <std::vector<int>> triRecords;
	


	//colliders
	std::vector<Plain*> plains;
	Ball* ball;

	//std::vector<Particle *> particles;


public:
	GLfloat RoundOffError;
	Cloth(GLfloat width, int pNum, glm::vec3 initPos,
		std::vector<Plain*> plains, Ball * ball, bool cross);
	~Cloth();

	//opengl draw function
	void update(GLfloat deltaTime, std::vector<Plain*> addtionalPlain);
	void draw(const glm::mat4& viewProjMtx, GLuint shader);
	void renderUpdate();
	
	//cloth
	void createCloth(GLfloat width, int pNum, glm::vec3 initPos, bool cross);
	glm::vec3 ComputeSpringForce(std::tuple<int, int, GLfloat> spring);
	glm::vec3 ComputeSpringHelper(glm::vec3 pos1, glm::vec3 pos2, 
		glm::vec3 vel1, glm::vec3 vel2, GLfloat restLength);
	glm::vec3 SumSpringForce(int index, glm::vec3 lastPos, glm::vec3 lastVec);
	void calNorm(glm::vec3 windVel, std::tuple<int, int, int> triangle);
	glm::vec3 ComputeAeroHelper(glm::vec3 windVel, 
		glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3,
		glm::vec3 vec1, glm::vec3 vec2, glm::vec3 vec3);
	glm::vec3 SumAeroForce(int index, glm::vec3 lastPos, glm::vec3 lastVec);

	//intergration
	std::vector<glm::vec3> intEular(int index , GLfloat timeStep);
	std::vector<glm::vec3> intRK(int index , GLfloat timeStep);
	glm::vec3 defaultForces();
	void collision(int index, GLfloat timeStep, std::vector<Plain*> addtionalPlain);
	void edgeCollision(GLfloat timeStep);
	void translate(GLfloat amount, glm::vec3 dir);

	//getter
	GLfloat* getGrav()		{ return &this->grav; }
	GLfloat* getGravTimes() { return &this->gravMult; }
	GLfloat* getAirResist()	{ return &this->airResist; }
	GLfloat* getRadius()	{ return &this->radius; }
	GLfloat* getFriction()	{ return &this->friction; }
	GLfloat* getElastic()	{ return &this->elastic; }
	GLfloat* getSpring()	{ return &this->springConst; }
	GLfloat* getDamp()	{ return &this->dampingConst; }
	void setEuler(bool val) { this->isEular = val; }

	//wind
	GLfloat* getWindX() { return &this->windVelocity.x; }
	GLfloat* getWindY() { return &this->windVelocity.y; }
	GLfloat* getWindZ() { return &this->windVelocity.z; }

	//glm::vec3 
	void restoreDefault();


	//helpers
	GLfloat getDegree(glm::vec3 vec1, glm::vec3 vec2) { 
		vec1 = glm::normalize(vec1);
		vec2 = glm::normalize(vec2);
		return glm::degrees(std::atan2(glm::length(glm::cross(vec1, vec2)),
			glm::dot(vec1, vec2))); 
	}
	
};

////////////////////////////////////////////////////////////////////////////////

#endif
