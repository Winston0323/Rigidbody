#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "core.h"

//#include <string>
//#include <vector>

class Object
{
protected:
	glm::mat4 model;
	glm::vec3 color;

public:
	glm::mat4 getModel() { return model; }
	glm::vec3 getColor() { return color; }

	virtual void draw(const glm::mat4& viewProjMtx, GLuint shader) = 0;
	virtual void update() = 0;
};

#endif

