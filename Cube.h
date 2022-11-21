#ifndef _CUBE_H_
#define _CUBE_H_

#include "core.h"
#include "Plain.h"

////////////////////////////////////////////////////////////////////////////////

class Cube
{
private:
	GLfloat size;
	glm::vec3 origin;
	Plain* left;
	Plain* right;
	Plain* top;
	Plain* bot;
	Plain* front;
	Plain* back;
	std::vector<Plain*> plains;
	std::vector<Plain*> colliders;


public:
	Cube(GLfloat size, glm::vec3 origin);
	~Cube();
	void draw(const glm::mat4& viewProjMtx, GLuint shader);
	std::vector<Plain*> checkHit(glm::vec3 position, GLfloat radius);
	Plain* getTop() { return top; }
	Plain* getBot() { return bot; }
	Plain* getLeft() { return left; }
	Plain* getRight() { return right; }
	Plain* getFront() { return front; }
	Plain* getBack() { return back; }
	std::vector<Plain*> getPlains() { return this->plains; }
	std::vector<Plain*> getCollider() { return this->colliders; }
};

////////////////////////////////////////////////////////////////////////////////

#endif
