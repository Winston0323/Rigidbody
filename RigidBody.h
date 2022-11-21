#ifndef _RIGIDBODY_H_
#define _RIGIDBODY_H_

#include "Object.h"
#include "Plain.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

class RigidBody : public Object
{
private:

	//render datas
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> localPos;
	std::vector<glm::vec3> normals;
	std::vector<glm::ivec3> faces;
	std::vector<std::pair<int,int>> edges;

	//renders	
	GLuint VAO, VBOvertex, VBOnormal, EBO;
	
	//centers
	glm::vec3 geoCenter;
	glm::vec3 massCenter;
	
	//varibles for intergration 
	GLfloat pointMass;
	GLfloat totalMass;
	std::vector<glm::vec3> forces;
	glm::vec3 frcOnMassCtr;
	glm::vec3 velocity;

	//angular momentum
	glm::mat3 inertia;
	glm::mat3 iInverse;
	glm::mat3 inertiaInit;
	glm::mat3 iInvInit;
	glm::mat3 rotMat;
	glm::quat rotQuat;
	glm::vec3 angVel;
	glm::vec3 LMom;

	//linear momentum
	glm::vec3 PMom;
	
	//environment
	GLfloat grav;
	GLfloat gravMult;
	glm::vec3 gravForce;

	//collision
	std::vector<Plain*> colliders;
	GLfloat elastic;

	//3 dimension
	GLfloat xMax;
	GLfloat xMin;
	GLfloat yMax;
	GLfloat yMin;
	GLfloat zMax;
	GLfloat zMin;


public:
	RigidBody(std::string objFilename, GLfloat pointSize);
	~RigidBody();

	void draw(const glm::mat4& viewProjMtx, GLuint shader);
	void update(GLfloat deltaTime);
	void update();

	void spin(float angle, glm::vec3 axis);
	void spinPerVetex(float angle, glm::vec3 axis);
	void objParser(string objFilename);
	void scale(float level);
	void translation(glm::vec3 destination);
	void translationPerVertex(glm::vec3 destination);
	void moveToWorldCenter();

	//getter
	glm::vec3 GetGeoCenter()	{ return this->geoCenter; }
	glm::vec3 GetMassCenter()	{ return this->massCenter; }
	glm::mat3 GetInertia()		{ return this->inertia; }
	GLfloat	GetXMax() { return this->xMax; }
	GLfloat	GetXMin() { return this->xMin; }
	GLfloat	GetYMax() { return this->yMax; }
	GLfloat	GetYMin() { return this->yMin; }
	GLfloat	GetZMax() { return this->zMax; }
	GLfloat	GetZMin() { return this->zMin; }

	//intergration
	std::vector<glm::vec3> intEular(GLfloat timeStep);
	std::vector<glm::vec3> localToWorld(glm::vec3 massCenter, glm::mat3 rotMat);
	void collision(GLfloat timeStep);
	void edgeCollision(GLfloat timeStep);

	//getter for IMGUI
	GLfloat* getVelX() { return &this->velocity.x; }
	GLfloat* getVelY() { return &this->velocity.y; }
	GLfloat* getVelZ() { return &this->velocity.z; }
	GLfloat* getAngVelX() { return &this->angVel.x; }
	GLfloat* getAngVelY() { return &this->angVel.y; }
	GLfloat* getAngVelZ() { return &this->angVel.z; }
	GLfloat* getMassCenterX() { return &this->massCenter.x; }
	GLfloat* getMassCenterY() { return &this->massCenter.y; }
	GLfloat* getMassCenterZ() { return &this->massCenter.z; }
	GLfloat* getGravMult() { return &this->gravMult; }
	
	//setters
	void setCollider(std::vector<Plain*> val) { this->colliders = val; }
	
	//set up helpers
	void restoreDefault();
	glm::vec3 defaultForce();
	std::vector<glm::vec3> defaultVertexForce();
	void updateThreeDegree();
	
};

#endif
