#ifndef Material_HPP
#define Material_HPP

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

class Material
{
public:
	bool normalLighting;
	Material();
	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);
	~Material();
	void setMatToShader(GLuint shader);

private:
	glm::vec3 ambient, diffuse, specular;
	float shininess;
	
};

#endif