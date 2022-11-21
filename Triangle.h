#pragma once
#ifndef _Triangle_H_
#define _Triangle_H_

#include "core.h"
//#include "Particle.h"

////////////////////////////////////////////////////////////////////////////////

class Triangle
{
private:
	Particle* p1;
	Particle* p2;
	Particle* p3;

public:
	Triangle(Particle * p1, Particle * p2 , Particle * p3);
	~Triangle();

	void update();
	void calNorm(glm::vec3 velocity, GLfloat density, GLfloat dragConst);
	
	
};

////////////////////////////////////////////////////////////////////////////////

#endif