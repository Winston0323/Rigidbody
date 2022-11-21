#include "Material.h"

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->shininess = shininess;
	this->normalLighting = false;
};
Material::Material() {
	this->ambient = glm::vec3(0,0,0);
	this->diffuse = glm::vec3(0, 0, 0);
	this->specular = glm::vec3(0, 0, 0);
	this->shininess = 0.0f;
	this->normalLighting = false;
}
Material::~Material()
{
}
void Material::setMatToShader(GLuint shader)
{
	if(normalLighting == false)
	{
		
		glUniform3fv(glGetUniformLocation(shader, "material.ambient"), 1, glm::value_ptr(ambient));
		glUniform3fv(glGetUniformLocation(shader, "material.diffuse"), 1, glm::value_ptr(diffuse));
		glUniform3fv(glGetUniformLocation(shader, "material.specular"), 1, glm::value_ptr(specular));
		glUniform1f(glGetUniformLocation(shader, "material.shininess"), this->shininess);
		glUniform1i(glGetUniformLocation(shader, "normalLighting"), 0);
	}
	if (normalLighting == true) 
	{
		glUniform1i(glGetUniformLocation(shader, "normalLighting"), 1);
	}
	
}