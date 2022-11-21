#ifndef _LOBBY_H_
#define _LOBBY_H_

#include "Object.h"
#include "Material.h"
#include "Plain.h"
#include "Box.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>




class Lobby
{
private:
	struct Particle {
		glm::vec3 position, velocity;
		float life;
		//void initialize();
		//void update(float dt);
	};
	glm::mat4 model;
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texs;
	std::vector<glm::ivec3> faces;
	Plain* plainN;
	Plain* plainS;
	Plain* plainE;
	Plain* plainW;
	Plain* plainEN;
	Plain* plainWN;
	Plain* plainES;
	Plain* plainWS;
	Box* box1;
	Box* box2;
	Box* box3;
	unsigned int texture;
	
	GLuint VAO, VBOvertex, VBOnormal, VBOTex, EBO;
	

	Material material;
	//glm::vec3 center, xAxis, yAxis, zAxis, viewPos;
	glm::mat4 view, projection;
	GLuint shader;
	std::vector<Collider*> colliders;


public:
	Lobby();
	~Lobby();

	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	void update();
	void spin(float angle, glm::vec3 axis);
	void objParser(std::string objFilename);
	void scale(float x, float y, float z);
	void centerize();
	std::vector<Collider*> getPlains() { return colliders; }
};

#endif