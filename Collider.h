#ifndef _COLLIDER_H_
#define _COLLIDER_H_

#include "core.h"

//#include <string>
//#include <vector>

class Collider
{
protected:
	glm::vec3 origin;
	glm::vec3 norm;
	GLfloat radius;
	bool isSphere;

public:
	virtual bool checkHit(glm::vec3 pos, glm::vec3 nextPos, GLfloat rad) = 0;
	virtual glm::vec3 getOrigin() = 0;
	virtual glm::vec3 getNorm() = 0 ;
	virtual bool getIsSphere() = 0;
	virtual bool getBox() = 0;
	virtual GLfloat getRadius() = 0;
};
#endif