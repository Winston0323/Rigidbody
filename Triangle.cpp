#include "Triangle.h"


////////////////////////////////////////////////////////////////////////////////

Triangle::Triangle(Particle * p1, Particle * p2, Particle * p3)
{
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
}

////////////////////////////////////////////////////////////////////////////////

Triangle::~Triangle()
{

}

////////////////////////////////////////////////////////////////////////////////
void Triangle::update()
{

}
////////////////////////////////////////////////////////////////////////////////
void Triangle::calNorm(glm::vec3 velocity , GLfloat density, GLfloat dragConst) {
	glm::vec3 p1Pos = p1->GetPos();
	glm::vec3 p2Pos = p2->GetPos();
	glm::vec3 p3Pos = p3->GetPos();

	//calculate normal for triangle
	glm::vec3 n = glm::cross(p2Pos - p1Pos, p3Pos - p1Pos);
	n = glm::normalize(n);

	p1->addNorm(n);
	p2->addNorm(n);
	p3->addNorm(n);
	//caculation surface velocity
	glm::vec3 vSurface = (p1->GetVelocity() + p2->GetVelocity() + p3->GetVelocity()) / 3.0f;
	glm::vec3 v = vSurface - velocity;
	if (glm::length(velocity) < 1e-3) {
		return;
	}
	//calculating area
	GLfloat area = 0.5f * glm::length(glm::cross(p2Pos - p1Pos, p3Pos - p1Pos));
	area = area * glm::abs(glm::dot(glm::normalize(velocity), n));
	//caculating aero force 
	glm::vec3 faero = 1.0f/3.0f * -0.5f * density * glm::length(v) * glm::length(v) * dragConst * area * n;
	//applying aero force
	p1->ApplyForce(faero);
	p2->ApplyForce(faero);
	p3->ApplyForce(faero);
}
