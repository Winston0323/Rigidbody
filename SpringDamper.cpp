#include "SpringDamper.h"


////////////////////////////////////////////////////////////////////////////////

SpringDamper::SpringDamper(Particle* p1, Particle* p2, GLfloat SConst, GLfloat DConst, GLfloat RLength)
{
	this->p1 = p1;
	this->p2 = p2;
	this->springConst = SConst;
	this->dampingConst = DConst;
	this->restLength = RLength;
	
	//recording restPos
	p1StartPos = p1->GetPos();
	p2StartPos = p2->GetPos();

}

////////////////////////////////////////////////////////////////////////////////

SpringDamper::~SpringDamper()
{

}

////////////////////////////////////////////////////////////////////////////////
void SpringDamper::ComputeForce() {
	
	//calculate spring force
	glm::vec3 p1CurPos = p1->GetPos();//collcet particle pos
	glm::vec3 p2CurPos = p2->GetPos();
	GLfloat currLength = glm::length(p2CurPos - p1CurPos);//calculate currLength

	//Hooke's law 
	glm::vec3 disPlcVec = (restLength - currLength) * glm::normalize(p2CurPos - p1CurPos);
	glm::vec3 fSpring = -(springConst) * disPlcVec;
	
	//caculating damper force
	glm::vec3 p1CurVec = p1->GetVelocity();//collcet particle velocity
	glm::vec3 p2CurVec = p2->GetVelocity();
	float vClose = glm::dot(p1CurVec - p2CurVec , glm::normalize(p2CurPos - p1CurPos));
	glm::vec3 fDamper = -dampingConst * vClose * glm::normalize(p2CurPos - p1CurPos);

	//apply force
	glm::vec3 force = fSpring + fDamper;
	p1->ApplyForce(force);
	p2->ApplyForce(-force);
}
////////////////////////////////////////////////////////////////////////////////