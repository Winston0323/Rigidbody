#pragma once
#ifndef _PLAIN_H_
#define _PLAIN_H_

#include "core.h"
#include "Collider.h"

////////////////////////////////////////////////////////////////////////////////

class Plain : public Collider
{
private:
	GLuint VAO;
	GLuint VBO_positions, VBO_normals, EBO;

	glm::mat4 model;
	glm::vec3 color;
	glm::vec3 origin;
	//bool isSphere;

	// Plain Information
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> refPos;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
	std::vector<std::pair<glm::vec3, glm::vec3>> edges;
	std::vector<Plain*> colliders;
	bool isBox = false;
	int lastHit = -1;
public:
	Plain(GLfloat size, glm::vec3 origin, glm::vec3 color);
	Plain(GLfloat size, glm::vec3 origin, glm::vec3 normal, glm::vec3 color);
	Plain(GLfloat size, glm::vec3 origin, GLfloat diff, glm::vec3 color, bool negative);
	Plain(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color);
	Plain(glm::vec3 norm, glm::vec3 pntOnPlain);
	~Plain();

	void draw(const glm::mat4& viewProjMtx, GLuint shader);
	void update();
	void spin(GLfloat deg, glm::vec3 axis);
	void setNorm(glm::vec3 norm) { this->norm = norm; }
	glm::vec3 getColor() { return color; }
	//bool checkHit(glm::vec3 pos, glm::vec3 nextPos, glm::vec3 vel, GLfloat radius);
	bool checkHit(glm::vec3 pos, glm::vec3 nextPos, GLfloat radius);
	std::pair<bool,GLfloat> checkHit(glm::vec3 pos, glm::vec3 nextPos, glm::vec3 vel, GLfloat radius,int hitIndex);
	glm::vec3 getOrigin() { return this->origin; }
	glm::vec3 getNorm() { return this->norm; }
	GLfloat getRadius() { return radius; }
	bool getIsSphere() { return isSphere; }
	bool getBox() { return false; }
	std::vector<std::pair<glm::vec3, glm::vec3>> getEdges() { return this->edges; }
	std::vector<Plain*> genCollider();
	void translation(glm::vec3 destination);
	void setLastHit(int hitIndex) { this->lastHit = hitIndex; }

};

////////////////////////////////////////////////////////////////////////////////

#endif
