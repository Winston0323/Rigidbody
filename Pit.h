#ifndef _PIT_H_
#define _PIT_H_

#include "core.h"
#include "Plain.h"

////////////////////////////////////////////////////////////////////////////////

class Pit
{
private:
	GLfloat size;
	glm::vec3 origin;
	Plain* leftHorz;
	Plain* rightHorz;
	Plain* leftVert;
	Plain* rightVert;
	std::vector<Plain*> plains;
	std::vector<Plain*> colliders;
	std::vector<std::pair<glm::vec3, glm::vec3>> edges;


public:
	Pit(GLfloat size, glm::vec3 origin, GLfloat pitSize);
	~Pit();
	void draw(const glm::mat4& viewProjMtx, GLuint shader);
	std::vector<Plain*> checkHit(glm::vec3 position, GLfloat radius);
	Plain* getLeftHorz() { return leftHorz; }
	Plain* getRightHorz() { return rightHorz; }
	Plain* getLeftVert() { return leftVert; }
	Plain* getRightVert() { return rightVert; }
	std::vector<Plain*> getPlains() { return this->plains; }
	std::vector<Plain*> getCollider() { return this->colliders; }
	std::vector<std::pair<glm::vec3, glm::vec3>> getEdges() { return this->edges; }
};

////////////////////////////////////////////////////////////////////////////////

#endif
