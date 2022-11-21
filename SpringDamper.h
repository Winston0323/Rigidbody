#ifndef _SPRINGDAMPER_H_
#define _SPRINGDAMPER_H_

#include "core.h";
#include "Particle.h"
////////////////////////////////////////////////////////////////////////////////

class SpringDamper
{
private:
	GLfloat springConst, dampingConst, restLength;
	Particle* p1;
	Particle* p2;
	glm::vec3 p1StartPos, p2StartPos;

public:
	SpringDamper(Particle * p1, Particle * p2, GLfloat SConst, GLfloat DConst, GLfloat RLength);
	~SpringDamper();
	void ComputeForce();
};

////////////////////////////////////////////////////////////////////////////////

#endif

