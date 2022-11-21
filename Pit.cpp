#include "Pit.h"
////////////////////////////////////////////////////////////////////////////////
Pit::Pit(GLfloat size, glm::vec3 origin, GLfloat pitSize)
{
	this->size = size;
	this->origin = origin;

	//left horizontal
	glm::vec3 topOrigin = glm::vec3(origin.x - pitSize / 2.0f - size / 2.0f, origin.y + size / 2.0f, origin.z);
	leftHorz = new Plain(size, topOrigin,glm::vec3(0, 1, 0), glm::vec3(0.9f, 0.9f, 0.5f));
	std::vector<Plain*> temp = leftHorz->genCollider();
	//colliders.insert(colliders.end(),temp.begin(), temp.end());

	//left vertical
	glm::vec3 leftOrigin = glm::vec3(origin.x-size, origin.y, origin.z);
	leftVert = new Plain(size, leftOrigin, glm::vec3(0.5, 0.5, 0), glm::vec3(0.6f, 0.1f, 0.3f));
	temp = leftVert->genCollider();
	colliders.insert(colliders.end(), temp.begin(), temp.end());

	//right horizontal
	glm::vec3 botOrigin = glm::vec3(origin.x + pitSize / 2.0f + size / 2.0f, origin.y + size / 2.0f, origin.z);
	rightHorz = new Plain(size, botOrigin, glm::vec3(0, 1, 0), glm::vec3(0.1f, 0.1f, 0.5f));
	temp = rightHorz->genCollider();
	//colliders.insert(colliders.end(), temp.begin(), temp.end());

	//right vertical
	glm::vec3 rightOrigin = glm::vec3(origin.x + pitSize / 2.0f + 1, origin.y, origin.z);
	rightVert = new Plain(size, rightOrigin, glm::vec3(-1, 0, 0), glm::vec3(0.4f, 0.9f, 0.7f));
	temp = rightVert->genCollider();
	colliders.insert(colliders.end(), temp.begin(), temp.end());
	//add all plain in scene
	//this->plains.push_back(leftHorz);
	//this->plains.push_back(rightHorz);
	this->plains.push_back(leftVert);
	this->plains.push_back(rightVert);

}

////////////////////////////////////////////////////////////////////////////////

Pit::~Pit()
{

}

////////////////////////////////////////////////////////////////////////////////
void Pit::draw(const glm::mat4& viewProjMtx, GLuint shader)
{
	for (Plain* p : plains) {
		p->draw(viewProjMtx, shader);
	}
}
////////////////////////////////////////////////////////////////////////////////
std::vector<Plain*> Pit::checkHit(glm::vec3 position, GLfloat radius) {
	std::vector<Plain*> result;
	if (position.x - radius <= -this->size / 2 + EPSILON) {
		//left
		result.push_back(leftHorz);
	}
	else if (position.x + radius >= this->size / 2 - EPSILON) {
		//right
		result.push_back(rightHorz);
	}
	else if (position.y + radius >= this->size / 2 - EPSILON) {
		//top
		result.push_back(leftVert);
	}
	else if (position.y - radius <= -this->size / 2 + EPSILON) {
		//bottom
		result.push_back(rightVert);
	}
	else {
		//printf("Inside box");

	}
	return result;
}
