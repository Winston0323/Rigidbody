#ifndef _ASTRONAUT_H_
#define _ASTRONAUT_H_

#include <vector>
#include <list> 
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cTime>
#include <algorithm>
#include <Math.h>
#include "Plain.h"

class Astronaut
{
private:
	glm::mat4 model;
	glm::mat4 rotModel;
	glm::mat4 transModel;
	glm::vec4 color;
	GLfloat radius;
	glm::vec3 center;
	glm::vec3 faceDir;
	int counter;

	std::vector<glm::vec4> colorArr;
	glm::vec4 black = glm::vec4(0.10f, 0.28f, 0.31f, 1.0f);
	glm::vec4 blue = glm::vec4(0.07f, 0.18f, 0.82f, 1.0f);
	glm::vec4 yellow = glm::vec4(0.96f, 0.96f, 0.34f, 1.0f);
	glm::vec4 orange = glm::vec4(0.95f, 0.49f, 0.05f, 1.0f);
	glm::vec4 red = glm::vec4(0.77f, 0.07f, 0.07f, 1.0f);
	glm::vec4 white = glm::vec4(0.84f, 0.91f, 0.94f, 1.0f);
	glm::vec4 lime = glm::vec4(0.31f, 0.94f, 0.22f, 1.0f);
	glm::vec4 brown = glm::vec4(0.44f, 0.29f, 0.114f, 1.0f);
	glm::vec4 pink = glm::vec4(0.93f, 0.33f, 0.733f, 1.0f);
	glm::vec4 cyan = glm::vec4(0.57f, 0.99f, 0.867f, 1.0f);
	
	//different set of position normals and faces
	std::vector<glm::vec3> sPoints, mPoints1, mPoints2, currPoints;
	std::vector<glm::vec3> sNorms, mNorms1, mNorms2, currNorms;
	std::vector<glm::ivec3> sFaces, mFaces1, mFaces2, currFaces;
	int lastMove = 1;

	GLuint VAO, VBOvertex, VBOnormal, EBO;


public:
	Astronaut();
	~Astronaut();

	//render
	void draw(const glm::mat4& viewProjMtx, GLuint shader, glm::vec3 eyePos);
	void render(const glm::mat4& view, const glm::mat4& projection, GLuint shader, GLuint particleShader,
		glm::vec3 viewPos, glm::vec3 lightDir);
	void update();
	void setModelMatrix(glm::mat4 mat);
	//parser
	void objParser(std::string objFilename, int parse);
	void parseAll();

	

	std::vector<glm::vec3> centerize(std::vector<glm::vec3> points);
	void centerizeAll();

	void spin(float angle, glm::vec3 axis);
	void rotation(float angle, glm::vec3 axis);
	void scale(float x, float y, float z);
	void translate(glm::vec3 destination);
	glm::vec3 GetPos() { return this->center; }
	int randNum(int min, int max);
	glm::vec4 randColor();
	

};

#endif

