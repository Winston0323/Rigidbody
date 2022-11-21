#ifndef _BOX_H_
#define _BOX_H_

#include "core.h"
#include "Plain.h"

////////////////////////////////////////////////////////////////////////////////

class Box
{
private:
	GLuint VAO;
	GLuint VBO_positions, VBO_normals, EBO;

	glm::mat4 model;
	glm::vec3 color;

	// Box Information
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> refPos;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
	std::vector<std::pair<int, int>> edgeRecord;
	std::vector<Plain*> colliders;
public:
	Box(glm::vec3 cubeMin = glm::vec3(-1, -1, -1), glm::vec3 cubeMax = glm::vec3(1, 1, 1));
	~Box();

	void draw(const glm::mat4& viewProjMtx, GLuint shader);
	void update();
	void setModel(glm::mat4 mat);
	void spin(float deg);
	void translation(glm::vec3 destination);
	std::vector<std::pair<int, int>> getEdgeRecord() { return this->edgeRecord; }
	glm::vec3 getPosition(int index) { return this->positions[index]; }
	std::vector<Plain*> getCollider() { return this->colliders; }
};

////////////////////////////////////////////////////////////////////////////////

#endif
