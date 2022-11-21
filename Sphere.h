#ifndef _SPHERE_H_
#define _SPHERE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Sphere {
public:

	glm::mat4 model;
	GLfloat radius;
	GLuint vao = 0, vbo = 0, vbo_n = 0, ebo = 0;

	int stackCount = 40;
	int sectorCount = 40;
	int numsToDraw;
	glm::vec3 color;
	Sphere(GLfloat radius, glm::vec3);

	void draw(const glm::mat4& projection, const glm::mat4& view, GLuint shader);
	void spin(float angle, glm::vec3 axis);
	void update();
	void scale(float level);
	void translation(glm::vec3 destination);
	GLfloat* getRadius() { return &this->radius; }
	void changeColor(glm::vec3 color);
};

#endif