#include "Astronaut.h"

Astronaut::Astronaut()
{
	parseAll();
	centerizeAll();

	
	colorArr.push_back(black);
	colorArr.push_back(blue);
	colorArr.push_back(yellow);
	colorArr.push_back(orange);
	colorArr.push_back(red);
	colorArr.push_back(white);
	colorArr.push_back(lime);
	colorArr.push_back(pink);
	colorArr.push_back(cyan);
	colorArr.push_back(brown);

	this->color = randColor();
	this->radius = 0.5f;
	this->faceDir = glm::vec3(0, 0, 1);
	this->counter = 0;
	model = glm::mat4(1);
	rotModel = glm::mat4(1);
	transModel = glm::mat4(1);
	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBOvertex);
	glGenBuffers(1, &VBOnormal);
	//for (int i = 0; i < 10; i++) {
	//	std::cout << points[i].x<< points[i].y << points[i].z << std::endl;
	//}
	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBOvertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * sPoints.size(), sPoints.data(), GL_STATIC_DRAW);

	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind VBO to the bound VAO, and store the normal data
	glBindBuffer(GL_ARRAY_BUFFER, VBOnormal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * sNorms.size(), sNorms.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	//generate ebo 
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * sFaces.size(), sFaces.data(), GL_STATIC_DRAW);


	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

Astronaut::~Astronaut()
{	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &VBOvertex);
	glDeleteBuffers(1, &VBOnormal);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);

}
void Astronaut::setModelMatrix(glm::mat4 mat) {
	this->model = mat * this->model;
}
void Astronaut::draw(const glm::mat4& viewProjMtx, GLuint shader, glm::vec3 eyePos) {
	model = transModel * rotModel;
	// actiavte the shader program 
	glUseProgram(shader);

	// get the locations and send the uniforms to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
	glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, glm::value_ptr(eyePos));
	glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);
	glUniform4fv(glGetUniformLocation(shader, "astColor"), 1, glm::value_ptr(this->color));

	// Bind the VAO
	glBindVertexArray(VAO);

	// draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, sFaces.size() * 3 , GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
	//update();

}

void Astronaut::render(const glm::mat4& view, const glm::mat4& projection, GLuint shader, GLuint particleShader,
	glm::vec3 viewPos, glm::vec3 lightDir) {
	

}
void Astronaut::update() {
	counter++;

	if (counter >= 30) {
		if (currPoints == sPoints) {
			if (lastMove == 1) {
				currPoints = mPoints2;
				currNorms = mNorms2;
				currFaces = mFaces2;
				lastMove = 2;
			}
			else {
				currPoints = mPoints1;
				currNorms = mNorms1;
				currFaces = mFaces1;
				lastMove = 1;
			}	
		}
		else if (currPoints == mPoints1) {
			currPoints = sPoints;
			currNorms = sNorms;
			currFaces = sFaces;
		
		}
		else {
			currPoints = sPoints;
			currNorms = sNorms;
			currFaces = sFaces;
		}
		// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBOvertex);
		glGenBuffers(1, &VBOnormal);
		//for (int i = 0; i < 10; i++) {
		//	std::cout << points[i].x<< points[i].y << points[i].z << std::endl;
		//}
		// Bind VAO
		glBindVertexArray(VAO);

		// Bind VBO to the bound VAO, and store the point data
		glBindBuffer(GL_ARRAY_BUFFER, VBOvertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * currPoints.size(), currPoints.data(), GL_STATIC_DRAW);

		// Enable Vertex Attribute 0 to pass point data through to the shader
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		// Bind VBO to the bound VAO, and store the normal data
		glBindBuffer(GL_ARRAY_BUFFER, VBOnormal);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * currNorms.size(), currNorms.data(), GL_STATIC_DRAW);
		// Enable Vertex Attribute 0 to pass point data through to the shader
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		//generate ebo 
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * currFaces.size(), currFaces.data(), GL_STATIC_DRAW);


		// Unbind the VBO/VAO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		counter = 0;
	}


}
void Astronaut::parseAll() {
	objParser("Obj/amongus_astro_still.obj", 1);
	objParser("Obj/amongus_astro_moving1.obj", 2);
	objParser("Obj/amongus_astro_moving2.obj", 3);
}
void Astronaut::objParser(std::string objFilename, int parse)
{
	std::ifstream objFile(objFilename); // The obj file we are reading.
	std::vector<glm::vec3> points, outPoints;
	std::vector<glm::vec3> normals, outNormals;
	std::vector<glm::vec3> textures;
	std::vector<glm::ivec3> faces, fNorms, outFaces;


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
				// Read the later three float numbers and use them as the 
				// coordinates.
				glm::vec3 point;
				ss >> point.x >> point.y >> point.z;

				// Process the point. For example, you can save it to a.
				points.push_back(point);
			}
			if (label == "vn")
			{
				// Read the later three float numbers and use them as the 
				// coordinates.
				glm::vec3 normal;
				ss >> normal.x >> normal.y >> normal.z;

				// Process the point. For example, you can save it to a.
				normals.push_back(normal);
			}
			if (label == "vt")
			{
				// Read the later three float numbers and use them as the 
				// coordinates.
				glm::vec3 texture;
				ss >> texture.x >> texture.y >> texture.z;

				// Process the point. For example, you can save it to a.
				textures.push_back(texture);
			}

			if (label == "f")
			{
				glm::ivec3 face;
				glm::ivec3 fNorm;
				std::string x, y, z;
				int xCord, yCord, zCord, xNorm, yNorm, zNorm;
				ss >> x >> y >> z;

				xCord = stoi(x.substr(0, x.find_first_of("/"))) - 1;
				yCord = stoi(y.substr(0, y.find_first_of("/"))) - 1;
				zCord = stoi(z.substr(0, z.find_first_of("/"))) - 1;
				xNorm = stoi(x.substr(x.find_last_of("/") + 1, x.size() - x.find_last_of("/") - 1)) - 1;
				yNorm = stoi(y.substr(y.find_last_of("/") + 1, y.size() - y.find_last_of("/") - 1)) - 1;
				zNorm = stoi(z.substr(z.find_last_of("/") + 1, z.size() - z.find_last_of("/") - 1)) - 1;
				//std::cout <<"xNorm="<< xNorm << "yNorm=" << yNorm << "zNorm= " << zNorm << std::endl;
				face.x = xCord;
				face.y = yCord;
				face.z = zCord;
				fNorm.x = xNorm;
				fNorm.y = yNorm;
				fNorm.z = zNorm;

				faces.push_back(face);
				fNorms.push_back(fNorm);
			}
		}
		int pos = 0;
		for (unsigned int i = 0; i < faces.size(); i++) {
			int pointPosX = faces[i].x;
			int pointPosY = faces[i].y;
			int pointPosZ = faces[i].z;
			int normPosX = fNorms[i].x;
			int normPosY = fNorms[i].y;
			int normPosZ = fNorms[i].z;
			//std::cout << pointPosX << " " << pointPosY << " " << pointPosZ << std::endl;
			//glm::vec3 outPoint = glm::vec3(, points[pointPosY], points.at(pointPosZ));
			outPoints.push_back(points[pointPosX]);
			outPoints.push_back(points[pointPosY]);
			outPoints.push_back(points[pointPosZ]);
			//glm::vec3 outNormal = glm::vec3(normals.at(normPosX), normals.at(normPosY), normals.at(normPosZ));
			outNormals.push_back(normals[normPosX]);
			outNormals.push_back(normals[normPosY]);
			outNormals.push_back(normals[normPosZ]);
			pos = outPoints.size();
			outFaces.push_back(glm::ivec3(pos - 3, pos - 2, pos - 1));

		}

		if ( parse == 1 ) {
			this->sPoints = outPoints;
			this->sNorms = outNormals;
			this->sFaces = outFaces;
		}
		else if (parse == 2) {
			this->mPoints1 = outPoints;
			this->mNorms1 = outNormals;
			this->mFaces1 = outFaces;
		
		}else if (parse == 3){
			this->mPoints2 = outPoints;
			this->mNorms2 = outNormals;
			this->mFaces2 = outFaces;
		}
	}
	else
	{
		std::cerr << "Can't open the file " << objFilename << std::endl;
	}
	objFile.close();
}
void Astronaut::spin(float angle, glm::vec3 axis) {
	rotModel = glm::rotate(glm::radians(angle), axis);
}
void Astronaut::rotation(float angle, glm::vec3 axis) {

	//translate to origin
	model = glm::rotate(glm::radians(angle), axis) *model;
	//translate back
}
void Astronaut::scale(float x, float y, float z) {
	model = glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z)) * model;
}
void Astronaut::translate(glm::vec3 destination)
{
	glm::vec4 center4 = glm::vec4(this->center, 1.0);
	center4 = glm::translate(glm::mat4(1.0f), destination) * center4;
	this->center = glm::vec3(center4.x, center4.y, center4.z );
	//model = glm::translate(glm::mat4(1.0f), destination) * model;
	transModel = glm::translate(glm::mat4(1.0f), destination);
}
void Astronaut::centerizeAll()
{
	this->sPoints = centerize(sPoints);
	this->mPoints1 = centerize(mPoints1);
	this->mPoints2 = centerize(mPoints2);
	this->center = glm::vec3(0.0f, 0.0f, 0.0f);
}
std::vector<glm::vec3> Astronaut::centerize(std::vector<glm::vec3> points)
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
	return points;
}
int Astronaut::randNum(int min, int max) {
	int rand = min + std::rand() % (max - min);
	return rand;
}
glm::vec4 Astronaut::randColor()
{
	glm::vec4 rCol = this->colorArr[randNum(0, colorArr.size())];
	return rCol;
}

