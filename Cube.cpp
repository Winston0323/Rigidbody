#include "Cube.h"
////////////////////////////////////////////////////////////////////////////////
Cube::Cube(GLfloat size, glm::vec3 origin)
{
	this->size = size;
	this->origin = origin;

	//top
	glm::vec3 topOrigin = glm::vec3(origin.x, origin.y + size / 2, origin.z);
	top = new Plain(size, topOrigin,glm::vec3(0,-1,0), glm::vec3(0.9f, 0.9f, 0.5f));
	std::vector<Plain*> temp = top->genCollider();
	colliders.insert(colliders.end(),temp.begin(), temp.end());

	//bottom
	glm::vec3 botOrigin = glm::vec3(origin.x, origin.y - size / 2, origin.z);
	bot = new Plain(size, botOrigin, glm::vec3(0, 1, 0), glm::vec3(0.1f, 0.1f, 0.5f));
	temp = bot->genCollider();
	colliders.insert(colliders.end(), temp.begin(), temp.end());

	//left 
	glm::vec3 leftOrigin = glm::vec3(origin.x - size / 2, origin.y, origin.z);
	left = new Plain(size, leftOrigin, glm::vec3(1, 0, 0), glm::vec3(0.6f, 0.1f, 0.3f));
	temp = left->genCollider();
	colliders.insert(colliders.end(), temp.begin(), temp.end());
	//right 
	glm::vec3 rightOrigin = glm::vec3(origin.x + size / 2, origin.y, origin.z);
	right = new Plain(size, rightOrigin, glm::vec3(-1, 0, 0), glm::vec3(0.4f, 0.9f, 0.7f));
	temp = right->genCollider();
	colliders.insert(colliders.end(), temp.begin(), temp.end());
	//front 
	glm::vec3 frontOrigin = glm::vec3(origin.x, origin.y, origin.z + size / 2);
	front = new Plain(size, frontOrigin, glm::vec3(0, 0, -1), glm::vec3(0.5f, 0.7f, 0.4f));
	temp = front->genCollider();
	colliders.insert(colliders.end(), temp.begin(), temp.end());
	//back 
	glm::vec3 backOrigin = glm::vec3(origin.x, origin.y, origin.z - size / 2);
	back = new Plain(size, backOrigin, glm::vec3(0, 0, 1), glm::vec3(0.5f, 0.3f, 0.6f));
	temp = back->genCollider();
	colliders.insert(colliders.end(), temp.begin(), temp.end());
	//add all plain in scene
	this->plains.push_back(top);
	this->plains.push_back(bot);
	this->plains.push_back(left);
	this->plains.push_back(right);
	this->plains.push_back(front);
	this->plains.push_back(back);

}

////////////////////////////////////////////////////////////////////////////////

Cube::~Cube()
{

}

////////////////////////////////////////////////////////////////////////////////
void Cube::draw(const glm::mat4& viewProjMtx, GLuint shader)
{
	for (Plain* p : plains) {
		p->draw(viewProjMtx, shader);
	}
}
////////////////////////////////////////////////////////////////////////////////
std::vector<Plain*> Cube::checkHit(glm::vec3 position, GLfloat radius) {
	std::vector<Plain*> result;
	if (position.x - radius <= -this->size / 2 + EPSILON) {
		//left
		result.push_back(left);
	}
	else if (position.x + radius >= this->size / 2 - EPSILON) {
		//right
		result.push_back(right);
	}
	else if (position.y + radius >= this->size / 2 - EPSILON) {
		//top
		result.push_back(top);
	}
	else if (position.y - radius <= -this->size / 2 + EPSILON) {
		//bottom
		result.push_back(bot);
	}
	else if (position.z + radius >= this->size / 2 - EPSILON) {
		//front
		result.push_back(front);
	}
	else if (position.z - radius <= -this->size / 2 + EPSILON) {
		//back
		result.push_back(back);
	}
	else {
		//printf("Inside box");

	}
	return result;
}
