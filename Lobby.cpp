#include "Lobby.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

Lobby::Lobby()
{
	glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
	plainE = new Plain(xAxis, glm::vec3(-15.0f, 0.0f, 0.0f));
	plainW = new Plain(-xAxis, glm::vec3(16.0f, 0.0f, 0.0f));
	plainS = new Plain(-zAxis, glm::vec3(0.0f, 0.0f, 16.5f));
	plainN = new Plain(zAxis, glm::vec3(0.0f, 0.0f, -17.0f));
	plainEN = new Plain(glm::normalize(glm::vec3(1.0f, 0.0f, -1.0f)), glm::vec3(-28.0f, 0.0f, 0.0f));
	plainWN = new Plain(glm::normalize(glm::vec3(-1.0f, 0.0f, -1.0f)), glm::vec3(29.0f, 0.0f, 0.0f));
	plainES = new Plain(glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)), glm::vec3(-10.0f, 0.0f, -10.0f));
	plainWS = new Plain(glm::normalize(glm::vec3(-1.0f, 0.0f, 1.0f)), glm::vec3(11.0f, 0.0f, -10.0f));
	box1 = new Box(glm::vec3(-9.0f, 0, 6.0f), 2.5);
	box2 = new Box(glm::vec3(11.5f, 0, 4.0f), 2.5);
	box3 = new Box(glm::vec3(-7.0, 0, -5.0f), 2);
	colliders.push_back(plainE);
	colliders.push_back(plainW);
	colliders.push_back(plainN);
	colliders.push_back(plainS);
	colliders.push_back(plainEN);
	colliders.push_back(plainWN);
	colliders.push_back(plainES);
	colliders.push_back(plainWS);
	colliders.push_back(box1);
	colliders.push_back(box2);
	colliders.push_back(box3);
	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1);
	objParser("obj/amongus_lobby.obj");


	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)

	// load and generate the texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("Obj/amongus_lobby.png", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);

	centerize();
	this->material = Material(glm::vec3(0.0, 0.5, 1.0),
		glm::vec3(0.50754, 0.50754, 0.50754),
		glm::vec3(0.508273, 0.508273, 0.508273),
		0.4f * 128);
	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBOvertex);
	glGenBuffers(1, &VBOnormal);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBOvertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);

	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind VBO to the bound VAO, and store the normal data
	glBindBuffer(GL_ARRAY_BUFFER, VBOnormal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	//// Bind VBO to the bound VAO, and store the normal data
	glBindBuffer(GL_ARRAY_BUFFER, VBOTex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texs.size(), texs.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(2);
	

	//generate ebo 
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * faces.size(), faces.data(), GL_STATIC_DRAW);


	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Lobby::~Lobby()
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &VBOvertex);
	glDeleteBuffers(1, &VBOnormal);
	//glDeleteBuffers(1, &VBOTex);
	glDeleteVertexArrays(1, &VAO);
}


void Lobby::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader) {
	// Actiavte the shader program 
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(this->model));

	glBindVertexArray(VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Draw the triangles
	//glDrawElements(GL_TRIANGLES, faces.size()*3,GL_UNSIGNED_INT, 0);
	//glPointSize(3);
	//glDrawArrays(GL_POINTS, 0, points.size());
	glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, 0);
	// Unbind the VAO and shader program
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glUseProgram(0);
	//box1->draw(projection, view, shader);
	//box2->draw(projection, view, shader);
	//box3->draw(projection, view, shader);
}

void Lobby::update()
{
}
void Lobby::scale(float x, float y, float z) {
	model = glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z)) * model;
}
void Lobby::spin(float angle, glm::vec3 axis)
{
	// Update the model matrix by multiplying a rotation matrix
	model = glm::rotate(glm::radians(angle), axis) * model;
}
void Lobby::centerize()
{
	std::vector<GLfloat> xCord;
	std::vector<GLfloat> yCord;
	std::vector<GLfloat> zCord;
	for (int i = 0; i < points.size(); i++) {

		xCord.push_back(points[i].x);
		yCord.push_back(points[i].y);
		zCord.push_back(points[i].z);

	}
	GLfloat midX = (*std::max_element(xCord.begin(), xCord.end()) + *std::min_element(xCord.begin(), xCord.end())) / 2;
	GLfloat midY = (*std::max_element(yCord.begin(), yCord.end()) + *std::min_element(yCord.begin(), yCord.end())) / 2;
	GLfloat midZ = (*std::max_element(zCord.begin(), zCord.end()) + *std::min_element(zCord.begin(), zCord.end())) / 2;
	GLfloat maxDist = 0;
	glm::vec3 maxPoint(0, 0, 0);
	for (int i = 0; i < points.size(); i++) {

		points[i].x = points[i].x - midX;
		points[i].y = points[i].y - midY;
		points[i].z = points[i].z - midZ;
	}
}
void Lobby::objParser(std::string objFilename)
{
	std::vector<glm::vec3> inputPoints;
	std::vector<glm::vec2> inputTextures;
	std::vector<glm::vec3> inputNormals;
	std::vector<glm::vec3> pointIndices;
	std::vector<glm::vec3> textureIndices;
	std::vector<glm::vec3> normalIndices;

	std::ifstream objFile(objFilename); // The obj file we are reading.

	// Check whether the file can be opened.
	if (objFile.is_open())
	{
		std::string line; // A line in the file.

		// Read lines from the file.
		while (std::getline(objFile, line))
		{
			// Turn the line into a string stream for processing.
			std::stringstream ss;
			ss << line;

			// Read the first word of the line.
			std::string label;
			ss >> label;

			// If the line is about vertex (starting with a "v").
			if (label == "v")
			{
				// Read the later three float numbers and use them as the coordinates.
				glm::vec3 point;
				ss >> point.x >> point.y >> point.z;

				// Process the point.
				inputPoints.push_back(point);
			}
			// If the line is about vertex texture (starting with a "vt").
			else if (label == "vt")
			{
				// Read the later three float numbers and use them as the coordinates.
				glm::vec2 texture;
				ss >> texture.x >> texture.y;

				// Process the texture.
				inputTextures.push_back(texture);
			}
			// If the line is about vertex normal (starting with a "vn").
			else if (label == "vn")
			{
				// Read the later three float numbers and use them as the coordinates.
				glm::vec3 normal;
				ss >> normal.x >> normal.y >> normal.z;

				// Process the normal.
				inputNormals.push_back(normal);
			}
			// If the line is about face (starting with a "f").
			else if (label == "f")
			{
				// Read the later three integers and use them as the indices.			
				std::string one, two, three;
				ss >> one >> two >> three;

				glm::ivec3 pIdx, tIdx, nIdx;
				pIdx.x = stoi(one.substr(0, one.find("/"))) - 1;
				one = one.substr(one.find("/") + 1);
				tIdx.x = stoi(one.substr(0, one.find("/"))) - 1;
				one = one.substr(one.find("/") + 1);
				nIdx.x = stoi(one) - 1;
				pIdx.y = stoi(two.substr(0, two.find("/"))) - 1;
				two = two.substr(two.find("/") + 1);
				tIdx.y = stoi(two.substr(0, two.find("/"))) - 1;
				two = two.substr(two.find("/") + 1);
				nIdx.y = stoi(two) - 1;
				pIdx.z = stoi(three.substr(0, three.find("/"))) - 1;
				three = three.substr(three.find("/") + 1);
				tIdx.z = stoi(three.substr(0, three.find("/"))) - 1;
				three = three.substr(three.find("/") + 1);
				nIdx.z = stoi(three) - 1;

				// Process the index.
				pointIndices.push_back(pIdx);
				textureIndices.push_back(tIdx);
				normalIndices.push_back(nIdx);
			}
		}
	}
	unsigned j = 0;
	unsigned numIndices = pointIndices.size();
	for (unsigned i = 0; i < numIndices; i++) {
		points.push_back(inputPoints[pointIndices[i].x]);
		texs.push_back(inputTextures[textureIndices[i].x]);
		normals.push_back(inputNormals[normalIndices[i].x]);

		points.push_back(inputPoints[pointIndices[i].y]);
		texs.push_back(inputTextures[textureIndices[i].y]);
		normals.push_back(inputNormals[normalIndices[i].y]);

		points.push_back(inputPoints[pointIndices[i].z]);
		texs.push_back(inputTextures[textureIndices[i].z]);
		normals.push_back(inputNormals[normalIndices[i].z]);

		faces.push_back(glm::vec3(j, j + 1, j + 2));
		j += 3;
	}
	//	int pos = 0;
	//	for (unsigned int i = 0; i < faces.size(); i++) {
	//		int pointPosX = faces[i].x;
	//		int pointPosY = faces[i].y;
	//		int pointPosZ = faces[i].z;
	//		int texPosX = fTexs[i].x;
	//		int texPosY = fTexs[i].y;
	//		int texPosZ = fTexs[i].z;
	//		int normPosX = fNorms[i].x;
	//		int normPosY = fNorms[i].y;
	//		int normPosZ = fNorms[i].z;
	//		//std::cout << pointPosX << " " << pointPosY << " " << pointPosZ << std::endl;
	//		//glm::vec3 outPoint = glm::vec3(, points[pointPosY], points.at(pointPosZ));
	//		outPoints.push_back(points[pointPosX]);
	//		outPoints.push_back(points[pointPosY]);
	//		outPoints.push_back(points[pointPosZ]);
	//		outTexs.push_back(textures[texPosX]);
	//		outTexs.push_back(textures[texPosY]);
	//		outTexs.push_back(textures[texPosZ]);
	//		//glm::vec3 outNormal = glm::vec3(normals.at(normPosX), normals.at(normPosY), normals.at(normPosZ));
	//		outNormals.push_back(normals[normPosX]);
	//		outNormals.push_back(normals[normPosY]);
	//		outNormals.push_back(normals[normPosZ]);
	//		pos = outPoints.size();
	//		outFaces.push_back(glm::ivec3(pos - 3, pos - 2, pos - 1));

	//	}
	//	this->points = outPoints;
	//	this->texs = outTexs;
	//	this->normals = outNormals;
	//	this->faces = outFaces;
	//}
	//else
	//{
	//	std::cerr << "Can't open the file " << objFilename << std::endl;
	//}
	objFile.close();
}