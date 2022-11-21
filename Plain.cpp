#include "Plain.h"


////////////////////////////////////////////////////////////////////////////////
Plain::Plain(GLfloat size, glm::vec3 origin, glm::vec3 color)
{
	this->origin = origin;
	// Model matrix.
	model = glm::mat4(1.0f);

	// The color of the cube. Try setting it to something else!
	this->color = color;

	// Specify vertex positions
	GLfloat offset = size / 2;
	positions = {
		glm::vec3(-offset + origin.x , origin.y, -offset + origin.z),//topleft
		glm::vec3(offset + origin.x, origin.y, -offset + origin.z),//topright
		glm::vec3(-offset + origin.x, origin.y, +offset + origin.z),//downleft
		glm::vec3(+offset + origin.x, origin.y, +offset + origin.z)//downright
	};

	// Specify normals
	normals = {
		glm::vec3(0,1,0),
		glm::vec3(0,1,0),
		glm::vec3(0,1,0),
		glm::vec3(0,1,0),

	};

	// Specify indices
	indices = {
		0,2,1,		1,2,3, // Front
	};

	//update edges
	edges.push_back(std::make_pair(0,2));
	edges.push_back(std::make_pair(1,2));
	edges.push_back(std::make_pair(0,1));
	edges.push_back(std::make_pair(2,3));
	edges.push_back(std::make_pair(1,3));
	
	// Generate a vertex array (VAO) and two vertex buffer objects (VBO).
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_positions);
	glGenBuffers(1, &VBO_normals);

	// Bind to the VAO.
	glBindVertexArray(VAO);

	// Bind to the first VBO - We will use it to store the vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind to the second VBO - We will use it to store the normals
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO and send the data
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// Unbind the VBOs.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
Plain::Plain(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color)
{
	this->origin = p1;
	model = glm::mat4(1.0f);
	this->isSphere = false;
	this->color = color;

	positions = {
		p1,
		p2,
		p3
	};
	refPos = {
		p1,
		p2,
		p3
	};
	//calculate normal for triangle
	this->norm = glm::normalize(glm::cross(positions[1] - positions[0],
		positions[2] - positions[0]));

	// Specify normals
	normals = {
		norm,
		norm,
		norm,
		norm
	};

	// Specify indices
	indices = {
		0,1,2
	};
}
////////////////////////////////////////////////////////////////////////////////
Plain::Plain(GLfloat size, glm::vec3 origin, glm::vec3 normal, glm::vec3 color)
{
	this->origin = origin;
	// Model matrix.
	model = glm::mat4(1.0f);

	// The color of the cube. Try setting it to something else!
	this->color = color;
	this->norm = normal;
	// Specify vertex positions
	GLfloat offset = size / 2;
	if (normal.x != 0 && normal.y != 0) {
		GLfloat temp = offset / sqrt(2);
		positions = {
			glm::vec3(origin.x + offset, origin.y - offset, +offset + origin.z),//topleft
			glm::vec3(origin.x - offset, origin.y + offset, +offset + origin.z),//topright
			glm::vec3(origin.x + offset, origin.y - offset, -offset + origin.z),//downleft
			glm::vec3(origin.x - offset, origin.y + offset, -offset + origin.z)//downright
			
		};
	}
	else {

		if (normal.y != 0) {

			if (normal.y > 0) {
				positions = {
					glm::vec3(-offset + origin.x, origin.y, -offset + origin.z),//topleft
					glm::vec3(+offset + origin.x, origin.y, -offset + origin.z),//topright
					glm::vec3(-offset + origin.x, origin.y, +offset + origin.z),//downleft
					glm::vec3(+offset + origin.x, origin.y, +offset + origin.z)//downright
				};
			}
			else {
				positions = {
					glm::vec3(-offset + origin.x, origin.y, +offset + origin.z),//topleft
					glm::vec3(+offset + origin.x, origin.y, +offset + origin.z),//topright
					glm::vec3(-offset + origin.x, origin.y, -offset + origin.z),//downleft
					glm::vec3(+offset + origin.x, origin.y, -offset + origin.z)//downright
				};

			}

		}
		else if (normal.x != 0) {
			if (normal.x > 0) {
				positions = {
					glm::vec3(origin.x, origin.y + offset, +offset + origin.z),//topleft
					glm::vec3(origin.x, origin.y + offset, -offset + origin.z),//topright
					glm::vec3(origin.x, origin.y - offset, +offset + origin.z),//downleft
					glm::vec3(origin.x, origin.y - offset, -offset + origin.z)//downright
				};
			}
			else {
				positions = {
					glm::vec3(origin.x, origin.y + offset, -offset + origin.z),//topleft
					glm::vec3(origin.x, origin.y + offset, +offset + origin.z),//topright
					glm::vec3(origin.x, origin.y - offset, -offset + origin.z),//downleft
					glm::vec3(origin.x, origin.y - offset,  +offset + origin.z)//downright
				};

			}
		}
		else {
			if (normal.z > 0) {
				positions = {
					glm::vec3(-offset + origin.x, +offset + origin.y, origin.z),//topleft
					glm::vec3(+offset + origin.x, +offset + origin.y, origin.z),//topright
					glm::vec3(-offset + origin.x, -offset + origin.y, origin.z),//downleft
					glm::vec3(+offset + origin.x, -offset + origin.y, origin.z)//downright
				};
			}
			else {
				positions = {
					glm::vec3(+offset + origin.x, +offset + origin.y, origin.z),//topleft
					glm::vec3(-offset + origin.x, +offset + origin.y, origin.z),//topright
					glm::vec3(+offset + origin.x, -offset + origin.y, origin.z),//downleft
					glm::vec3(-offset + origin.x, -offset + origin.y, origin.z)//downright
				};

			}
		}
	}
	// Specify normals
	normal = glm::normalize(normal);
	normals = {
		normal,
		normal,
		normal,
		normal,

	};

	// Specify indices
	indices = {
		0,2,1,		1,2,3, // Front
	};

	//update edges
	edges.push_back(std::make_pair(0, 2));
	edges.push_back(std::make_pair(1, 2));
	edges.push_back(std::make_pair(0, 1));
	edges.push_back(std::make_pair(2, 3));
	edges.push_back(std::make_pair(1, 3));
	this->colliders = genCollider();
	//colliders.push_back(new Plain(	this->positions[this->indices[0]],
	//								this->positions[this->indices[1]],
	//								this->positions[this->indices[2]], this->color));

	//colliders.push_back(new Plain(	this->positions[this->indices[3]],
	//								this->positions[this->indices[4]],
	//								this->positions[this->indices[5]], this->color));
	
	// Generate a vertex array (VAO) and two vertex buffer objects (VBO).
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_positions);
	glGenBuffers(1, &VBO_normals);

	// Bind to the VAO.
	glBindVertexArray(VAO);

	// Bind to the first VBO - We will use it to store the vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind to the second VBO - We will use it to store the normals
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO and send the data
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// Unbind the VBOs.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
Plain::Plain(GLfloat size, glm::vec3 origin, GLfloat diff, glm::vec3 color, bool negative)
{
	this->origin = origin;
	// Model matrix.
	model = glm::mat4(1.0f);
	this->isSphere = false;

	// The color of the cube. Try setting it to something else!
	this->color = color;
	//color = glm::vec3(0.3f, 0.3f, 0.4f);

	// Specify vertex positions
	GLfloat offset = size / 2;
	positions = {
		
		glm::vec3(-offset + origin.x , origin.y - diff, -offset + origin.z),//topleft
		glm::vec3(-offset + origin.x, origin.y - diff, +offset + origin.z),//downleft
		glm::vec3(+offset + origin.x, origin.y + diff, +offset + origin.z),//downright
		glm::vec3(offset + origin.x, origin.y + diff, -offset + origin.z)//topright
	};
	//calculate normal for triangle
	this->norm = glm::normalize(glm::cross(positions[1] - positions[0], 
		positions[3] - positions[0]));
	if (negative) {
		this->norm = -this->norm;
		// Specify indices
		indices = {
			0,1,3,	1,2,3 // Front
		};
	}

	// Specify normals
	normals = {
		
		//glm::vec3(0,1,0),
		//glm::vec3(0,1,0),
		//glm::vec3(0,1,0),
		//glm::vec3(0,1,0),
		norm,
		norm,
		norm,
		norm

	};

	// Specify indices
	indices = {
		0,1,3,	1,2,3 // Front
	};

	// Generate a vertex array (VAO) and two vertex buffer objects (VBO).
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_positions);
	glGenBuffers(1, &VBO_normals);

	// Bind to the VAO.
	glBindVertexArray(VAO);

	// Bind to the first VBO - We will use it to store the vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind to the second VBO - We will use it to store the normals
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO and send the data
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	// Unbind the VBOs.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Plain::Plain(glm::vec3 norm, glm::vec3 pntOnPlain)
{
	model = glm::mat4(1.0f);
	this->color = glm::vec3(1.0f, 0.0f, 0.0f);
	this->norm = norm;
	this->origin = pntOnPlain;
	this->isSphere = false;
	this->radius = 0;
}
////////////////////////////////////////////////////////////////////////////////

Plain::~Plain()
{
	// Delete the VBOs and the VAO.
	glDeleteBuffers(1, &VBO_positions);
	glDeleteBuffers(1, &VBO_normals);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

////////////////////////////////////////////////////////////////////////////////

void Plain::draw(const glm::mat4& viewProjMtx, GLuint shader)
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// actiavte the shader program 
	glUseProgram(shader);

	// get the locations and send the uniforms to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
	glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);

	// Bind the VAO
	glBindVertexArray(VAO);

	// draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

////////////////////////////////////////////////////////////////////////////////

void Plain::update()
{

}
////////////////////////////////////////////////////////////////////////////////
void Plain::spin(GLfloat angle, glm::vec3 axis)
{
	model = glm::translate(origin) * glm::rotate(glm::radians(angle), axis) * glm::translate(-origin) * model;
}
bool Plain::checkHit(glm::vec3 pos, glm::vec3 nextPos, GLfloat rad)
{
	
	GLfloat Zn = glm::dot((pos - this->origin), this->norm);
	GLfloat Znone = glm::dot((nextPos - this->origin), this->norm);
	if (Zn * Znone <= 0) {
		return true;

	}else{
		return false;
	}
}

////////////////////////////////////////////////////////////////////////////////
std::pair<bool, GLfloat> Plain::checkHit(glm::vec3 pos, glm::vec3 nextPos, glm::vec3 vel, GLfloat radius)
{
	//pos = pos - this->norm * radius;
	//GLfloat thit = glm::dot((this->origin - pos), this->norm) / glm::dot(vel, this->norm);
	if (glm::dot((pos - this->origin), this->norm) < 0) {
		return std::make_pair(false, -1.0f);;
	
	}


	GLfloat Zn = glm::dot((pos - this->origin), this->norm);
	GLfloat Znone = glm::dot((nextPos - this->origin), this->norm);
	if (Zn * Znone > 0) {
		return std::make_pair(false,-1.0f);
	}

	GLfloat thit = (Zn / (Zn - Znone)) * (1 / DEFAULT_SIMRATE);
	if (thit < (1 / DEFAULT_SIMRATE - EPSILON) && thit >= EPSILON) {

		glm::vec3 xhit = pos + thit * vel;
		glm::vec2 xhitTD;
		std::vector<glm::vec2> pjtPoint;
		//project to 2D
		if (std::abs(this->norm.x) > std::abs(this->norm.y) && std::abs(this->norm.x) > std::abs(this->norm.z)) {
			for (int i = 0; i < positions.size(); i++) {
				pjtPoint.push_back(glm::vec2(positions[i].y, positions[i].z));
			}
			xhitTD = glm::vec2(xhit.y, xhit.z);

		}
		else if (std::abs(this->norm.y) > std::abs(this->norm.x) && std::abs(this->norm.y) > std::abs(this->norm.z))
		{
			for (int i = 0; i < positions.size(); i++) {
				pjtPoint.push_back(glm::vec2(positions[i].z, positions[i].x));
			}
			xhitTD = glm::vec2(xhit.z, xhit.x);
		}
		else if (std::abs(this->norm.z) > std::abs(this->norm.x) && std::abs(this->norm.z) > std::abs(this->norm.y))
		{
			for (int i = 0; i < positions.size(); i++) {
				pjtPoint.push_back(glm::vec2(positions[i].x, positions[i].y));
			}
			xhitTD = glm::vec2(xhit.x, xhit.y);
		}
		else {
			for (int i = 0; i < positions.size(); i++) {
				pjtPoint.push_back(glm::vec2(positions[i].y, positions[i].z));
			}
			xhitTD = glm::vec2(xhit.y, xhit.z);
		}
		//comput edge vector
		std::vector<glm::vec2> edgeVec;
		std::vector<glm::vec2> hitVec;
		std::vector<glm::mat2> matrix;
		GLfloat ref = 0;
		for (int i = 0; i < pjtPoint.size(); i++) {
			glm::vec2 edge;
			if (i == pjtPoint.size() - 1) {
				edge = pjtPoint[0] - pjtPoint[i];
			}
			else {
				edge = pjtPoint[i + 1] - pjtPoint[i];
			}

			glm::vec2 hit = xhitTD - pjtPoint[i];
			glm::mat2 mat;
			mat[0][0] = edge.x;
			mat[1][0] = edge.y;
			mat[0][1] = hit.x;
			mat[1][1] = hit.y;

			GLfloat det = glm::determinant(mat);
			if (det * ref < 0) {//different sign
				return std::make_pair(false, -1.0f);
			}
			if (det != 0) {
				ref = det;
			}
		}
		return std::make_pair(true, thit);
		//comput 

	}
	else {

		return std::make_pair(false, -1.0f);
	}
}

std::vector<Plain*> Plain::genCollider() 
{
	std::vector<Plain*> colliders;
	for (int i = 0; i < this->indices.size(); i=i+3) {
		int curr = indices[i];
		int next = indices[i+1];
		int nn = indices[i+2];

		Plain* plain = new Plain(
			this->positions[curr], 
			this->positions[next], 
			this->positions[nn],
			this->color);
		colliders.push_back(plain);
	}
	return colliders;
}
void Plain::translation(glm::vec3 destination)
{
	//this->model = glm::translate(glm::mat4(1.0f), destination);
	for (int i = 0; i < this->positions.size(); i++) {
		this->positions[i] = glm::vec3(glm::translate(glm::mat4(1.0f), destination) * glm::vec4(this->refPos[i], 1));
		//this->colliders->translate
	}
}