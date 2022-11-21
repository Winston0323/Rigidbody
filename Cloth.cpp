#include "Cloth.h"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         #include "Cloth.h"



////////////////////////////////////////////////////////////////////////////////

Cloth::Cloth(GLfloat width, int pNum, glm::vec3 initPos, std::vector<Plain*> plains, Ball* ball, bool cross)
{
	//initialize parameter
	this->ball = ball;
	this->model = glm::mat4(1.0f);
	color = glm::vec3(1.0f, 0.95f, 0.1f);
	this->pNum = pNum;
	restoreDefault();
	this->plains = plains;
	this->gravForce = glm::vec3(0, -this->mass * this->grav * this->gravMult, 0);
	createCloth(width, pNum, initPos, cross);
	
	this->RoundOffError = 0;

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

////////////////////////////////////////////////////////////////////////////////
Cloth::~Cloth()
{
	// Delete the VBOs and the VAO.
	glDeleteBuffers(1, &VBO_positions);
	glDeleteBuffers(1, &VBO_normals);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}
////////////////////////////////////////////////////////////////////////////////
void Cloth::update(GLfloat deltaTime, std::vector<Plain*> addtionalPlain)
{
	//caclculate gravForce
	this->gravForce = glm::vec3(0, -this->mass * this->grav * this->gravMult, 0);
	edgeCollision(deltaTime);
	//update position data 
	#pragma omp parallel
	#pragma omp for
	//TOD
	for (int i = 0; i < positions.size(); i++) {
		if (isFixed[i] != true) {
			collision(i, deltaTime, addtionalPlain);
			//positions[i] = glm::vec3(positions[i].x, positions[i].y - 0.01, positions[i].z);
		}
	}
	//calculate spring forces
	for (int i = 0; i < springs.size(); i++)
	{
		this->forces[std::get<0>(springs[i])] = this->forces[std::get<0>(springs[i])] + ComputeSpringForce(springs[i]);
		this->forces[std::get<1>(springs[i])] = this->forces[std::get<1>(springs[i])] - ComputeSpringForce(springs[i]);
	}
	//calculate triangle forces
	for (int i = 0; i < triangles.size(); i++)
	{
		calNorm(this->windVelocity, triangles[i]);
	}
}
////////////////////////////////////////////////////////////////////////////////
void Cloth::draw(const glm::mat4& viewProjMtx, GLuint shader)
{
	glDisable(GL_CULL_FACE);
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
void Cloth::createCloth(GLfloat width, int pNum, glm::vec3 initPos, bool cross)
{
	//setting up positions
	GLfloat partDist = width / (pNum - 1);//caculate particle distant
	for (int j = 0; j < pNum; j++) {
		for (int i = 0; i < pNum; i++) {

			int currIndex = j * pNum + i;//current
			GLfloat xPos = initPos.x + partDist * i;
			GLfloat yPos = initPos.y - partDist * j;
			GLfloat zPos = initPos.z;
			
			//initialized vectors
			positions.push_back(glm::vec3(xPos, yPos, zPos));
			normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
			velocitys.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			forces.push_back(defaultForces());
			isFixed.push_back(false);

			//setup records			
			std::vector<int> temp;
			springRecords.push_back(temp);
			std::vector<int> temp1;
			triRecords.push_back(temp);
		}
	}
	
	//setting up fixed particles
	isFixed[0] = true;
	isFixed[pNum - 1] = true;

	//setting spring dampers
	for (int j = 0; j < pNum; j++) {//row num
		for (int i = 0; i < pNum; i++) {//col num

			int currIndex = j * pNum + i;//current
			if (i + 1 < pNum) {
				int nextIndex = j * pNum + i + 1;//right	
				std::tuple<int, int, GLfloat> sd = std::make_tuple(currIndex, nextIndex, partDist);
				springs.push_back(sd);
				springRecords[currIndex].push_back(springs.size() - 1);
				springRecords[nextIndex].push_back(springs.size() - 1);
				std::pair<int, int> edge = std::make_pair(currIndex,nextIndex);
			}

			if (j + 1 < pNum) {
				int nextIndex = (j + 1) * pNum + i;//lower
				std::tuple<int, int, GLfloat> sd = std::make_tuple(currIndex, nextIndex, partDist);
				springs.push_back(sd);
				springRecords[currIndex].push_back(springs.size() - 1);
				springRecords[nextIndex].push_back(springs.size() - 1);
				std::pair<int, int> edge = std::make_pair(currIndex, nextIndex);
			}
			if (cross) {
				if (i + 1 < pNum && j + 1 < pNum) {
					int nextIndex = (j + 1) * pNum + i + 1;//right diagnal
					std::tuple<int, int, GLfloat> sd = std::make_tuple(currIndex, nextIndex, partDist * sqrt(2));
					springs.push_back(sd);
					springRecords[currIndex].push_back(springs.size() - 1);
					springRecords[nextIndex].push_back(springs.size() - 1);
					std::pair<int, int> edge = std::make_pair(currIndex, nextIndex);
				}

				if (i != 0 && j + 1 < pNum) {//when not on the leftest side
					int nextIndex = (j + 1) * pNum + i - 1;//leftDiag
					std::tuple<int, int, GLfloat> sd = std::make_tuple(currIndex, nextIndex, partDist * sqrt(2));
					springs.push_back(sd);
					springRecords[currIndex].push_back(springs.size() - 1);
					springRecords[nextIndex].push_back(springs.size() - 1);
					std::pair<int, int> edge = std::make_pair(currIndex, nextIndex);
				}
			}
			else 
			{
				if (i + 2 < pNum) {
					int nextIndex = j * pNum + i + 2;//right diagnal
					std::tuple<int, int, GLfloat> sd = std::make_tuple(currIndex, nextIndex, partDist *2.0f);
					springs.push_back(sd);
					springRecords[currIndex].push_back(springs.size() - 1);
					springRecords[nextIndex].push_back(springs.size() - 1);
					std::pair<int, int> edge = std::make_pair(currIndex, nextIndex);
				}

				if (j + 2 < pNum) {//when not on the leftest side
					int nextIndex = (j + 2) * pNum + i;//leftDiag
					std::tuple<int, int, GLfloat> sd = std::make_tuple(currIndex, nextIndex, partDist * 2.0f);
					springs.push_back(sd);
					springRecords[currIndex].push_back(springs.size() - 1);
					springRecords[nextIndex].push_back(springs.size() - 1);
					std::pair<int, int> edge = std::make_pair(currIndex, nextIndex);
				}

			}
		}
	}
	//setting triangles
	for (int j = 0; j < pNum - 1; j++) {//row num
		for (int i = 0; i < pNum - 1; i++) {//col num
			//recording points
			int curr = j * pNum + i;//current
			int next = j * pNum + i + 1;//right
			int bottom = (j + 1) * pNum + i;//bottom
			int diag = (j + 1) * pNum + i + 1;//diagnal1

			//making triangle
			//Triangle* temp = new Triangle(curr, diag, next);
			std::tuple<int, int, int> temp = std::make_tuple(curr, diag, next);
			triangles.push_back(temp);
			this->indices.push_back(curr);
			this->indices.push_back(diag);
			this->indices.push_back(next);
			triRecords[curr].push_back(triangles.size() - 1);
			triRecords[diag].push_back(triangles.size() - 1);
			triRecords[next].push_back(triangles.size() - 1);
			//temp = new Triangle(curr, bottom, diag);
			temp = std::make_tuple(curr, bottom, diag);
			triangles.push_back(temp);
			this->indices.push_back(curr);
			this->indices.push_back(bottom);
			this->indices.push_back(diag);
			triRecords[curr].push_back(triangles.size() - 1);
			triRecords[bottom].push_back(triangles.size() - 1);
			triRecords[diag].push_back(triangles.size() - 1);
		}
	}

}
////////////////////////////////////////////////////////////////////////////////
std::vector<glm::vec3> Cloth::intEular(int index, GLfloat timeStep) {
	//get and check next state
	glm::vec3 acc = this->forces[index] / this->mass;
	//calculate x(n+1)
	glm::vec3 nextPos = this->positions[index] + velocitys[index] * timeStep;
	glm::vec3 nextVel = velocitys[index] + acc * timeStep;
	std::vector<glm::vec3> result;
	result.push_back(nextPos);
	result.push_back(nextVel);
	result.push_back(acc);	
	return result;
}
////////////////////////////////////////////////////////////////////////////////
glm::vec3 Cloth::SumSpringForce(int vetexInd, glm::vec3 lastPos, glm::vec3 lastVec) {
	glm::vec3 force = gravForce;
	//for each index inside the spring records
	for (int springIndex : springRecords[vetexInd]) {
		//get current spring data
		std::tuple<int, int, GLfloat> currSpring = springs[springIndex];
		int index0 = std::get<0>(currSpring);//first vertex index
		int index1 = std::get<1>(currSpring);//second vertex index
		GLfloat length = std::get<2>(currSpring);//spring rest lenght

		if (index0 == vetexInd) {//when index0 is the current index
			force = force + ComputeSpringHelper(lastPos, this->positions[index1],
				lastVec, this->velocitys[index1], length);
		}
		else if (index1 == vetexInd) {
			force = force - ComputeSpringHelper(this->positions[index0], lastPos,
				this->velocitys[index0], lastVec, length);
		}
		else {//error detected

			std::cout << "WRONG SPRING SELECTED!!!!!!!!!!!!!!!!!!!!" << std::endl;
		}
	}
	return force;
}
////////////////////////////////////////////////////////////////////////////////
glm::vec3 Cloth::SumAeroForce(int vetexInd, glm::vec3 lastPos, glm::vec3 lastVec) {
	glm::vec3 force =glm::vec3(0);
	//for each index inside the spring records
	#pragma omp parallel
	#pragma omp for
	for (int i = 0; i < triRecords[vetexInd].size(); i++) {
		//get current spring data
		int triIndex = triRecords[vetexInd][i];
		std::tuple<int, int, GLfloat> currTriangle = triangles[triIndex];
		int index1 = std::get<0>(currTriangle);//first vertex index
		int index2 = std::get<1>(currTriangle);//second vertex index
		int index3 = std::get<2>(currTriangle);//spring rest lenght
		glm::vec3 p1Pos = this->positions[index1];
		glm::vec3 p2Pos = this->positions[index2];
		glm::vec3 p3Pos = this->positions[index3];

		if (index1 == vetexInd) {//when index0 is the current index
			GLfloat protion = abs(getDegree(p2Pos - lastPos, p3Pos - lastPos) / 180.0f);
			force = force + protion * ComputeAeroHelper(this->windVelocity,
				lastPos, this->positions[index2], this->positions[index3],
				lastVec, this->velocitys[index2], this->velocitys[index3]);
		}
		else if (index2 == vetexInd) {
			GLfloat protion = abs(getDegree(p1Pos - lastPos, p3Pos - lastPos) / 180.0f);
			force = force + protion * ComputeAeroHelper(this->windVelocity,
				this->positions[index1], lastPos, this->positions[index3],
				this->velocitys[index1], lastVec,  this->velocitys[index3]);
		}
		else if (index3 == vetexInd) {
			GLfloat protion = abs(getDegree(p2Pos - lastPos, p1Pos - lastPos) / 180.0f);
			force = force + protion * ComputeAeroHelper(this->windVelocity,
				this->positions[index1], this->positions[index2], lastPos,
				this->velocitys[index1], this->positions[index2], lastVec);
		}
		else {//error detected

			std::cout << "WRONG Triangle SELECTED!!!!!!!!!!!!!!!!!!!!" << std::endl;
		
		}
	}
	return force;
}
////////////////////////////////////////////////////////////////////////////////
std::vector<glm::vec3> Cloth::intRK(int index, GLfloat timeStep) {
	//get and check next state
	glm::vec3 K1Acc = this->forces[index] / this->mass;
	//calculate x(n+1)
	glm::vec3 K1Pos = this->positions[index] + velocitys[index] * timeStep;
	glm::vec3 K1Vec = this->velocitys[index] + K1Acc * timeStep;
	
	//loop through all springs that the index is connected
	glm::vec3 K2force = SumSpringForce(index, K1Pos, K1Vec) + SumAeroForce(index, K1Pos, K1Vec);
	glm::vec3 K2Acc = K2force / this->mass;
	glm::vec3 K2Pos = this->positions[index] + K1Vec * 0.5f * timeStep;
	glm::vec3 K2Vec = this->velocitys[index] + K2Acc * 0.5f * timeStep;
	
	//loop through all springs that the index is connected
	glm::vec3 K3force = SumSpringForce(index, K2Pos, K2Vec) + SumAeroForce(index, K2Pos, K2Vec);
	glm::vec3 K3Acc = K3force / this->mass;
	glm::vec3 K3Pos = this->positions[index] + K2Vec * 0.5f * timeStep;
	glm::vec3 K3Vec = this->velocitys[index] + K3Acc * 0.5f * timeStep;
	
	//loop through all springs that the index is connected
	glm::vec3 K4force = SumSpringForce(index, K3Pos, K3Vec) + SumAeroForce(index, K3Pos, K3Vec);
	glm::vec3 K4Acc = K4force / this->mass;
	glm::vec3 K4Pos = this->positions[index] + K3Vec * timeStep;
	glm::vec3 K4Vec = this->velocitys[index] + K4Acc * timeStep;

	glm::vec3 nextPos = this->positions[index] + 
		(timeStep / 6.0f) * (K1Vec + 2.0f * K2Vec + 2.0f * K3Vec + K4Vec);

	glm::vec3 nextVel = this->velocitys[index] +
		(timeStep / 6.0f) * (K1Acc + 2.0f * K2Acc + 2.0f * K3Acc + K4Acc);

	std::vector<glm::vec3> result;
	result.push_back(nextPos);
	result.push_back(nextVel);
	result.push_back(K4Acc);
	return result;
}
////////////////////////////////////////////////////////////////////////////////
void Cloth::edgeCollision(GLfloat timeStep) {
	//get all edges
	for (std::tuple<int, int, GLfloat> currSpring : springs) {
		int index1 = std::get<0>(currSpring);//first vertex index
		int index2 = std::get<1>(currSpring);//second vertex index

		//edge
		glm::vec3 Q1 = this->positions[index1];
		glm::vec3 Q2 = this->positions[index2];
		glm::vec3 b = Q2 - Q1;
		
		//updated egde
		std::vector<glm::vec3> result1 = intRK(index1, timeStep);
		glm::vec3 Q1New = result1[0];
		glm::vec3 Q1VelNew = result1[1];
		std::vector<glm::vec3> result2 = intRK(index2, timeStep);
		glm::vec3 Q2New = result2[0];
		glm::vec3 Q2VelNew = result2[1];
		glm::vec3 bNew = Q2New - Q1New;

		//get obsticle edges
		for (std::pair<glm::vec3, glm::vec3> edgepair : ball->getEgde()) {
			//obsticle edges
			glm::vec3 P1 = edgepair.first;
			glm::vec3 P2 = edgepair.second;
			glm::vec3 a = P2 - Q1;

			//nhat between two edges
			if (glm::dot(a, b) <= EPSILON) {
				continue;
			}
			
			glm::vec3 nhat = glm::normalize(glm::cross(a, b));
			
			glm::vec3 r = Q1 - P1;
			glm::vec3 rNew = Q1New - P1;

			//point on obsticle edge
			GLfloat s = glm::dot(r, glm::cross(glm::normalize(b), nhat)) /
				glm::dot(a, glm::cross(glm::normalize(b), nhat));
			
			//point on colliding edge
			GLfloat t = glm::dot(-r, glm::cross(glm::normalize(a), nhat)) /
				glm::dot(b, glm::cross(glm::normalize(a), nhat));
			//updated collding edge
			GLfloat tNew = glm::dot(-r, glm::cross(glm::normalize(a), nhat)) /
				glm::dot(bNew, glm::cross(glm::normalize(a), nhat));
			if (s < 0 || s > 1 || t < 0 || t > 1 ) {
				continue;
			}
			else {
				//calculate point on edges
				glm::vec3 pa = P1 + s * a;
				glm::vec3 qa = Q1 + t * b;
				//updated point
				glm::vec3 qaNew = Q1New + tNew * bNew;
				//calculate m 
				glm::vec3 mOld = qa - pa;
				glm::vec3 mNew = qaNew - pa;

				if (glm::length(mOld) == TOLERANCE || glm::dot(mOld, mNew) < 0) {
					GLfloat lenghtP = glm::length(P2 - P1);
					GLfloat lenghtP1 = glm::length(pa - P1);
					GLfloat u = lenghtP1 / lenghtP;
					GLfloat v = 1.0f - u;
					glm::vec3 nextVel = Q1VelNew * u + Q2VelNew * v;
					glm::vec3 posCol = pa;
					glm::vec3 vn = glm::dot(nextVel, nhat) * nhat;
					glm::vec3 vt = nextVel - vn;
					glm::vec3 velCol = -this->elastic * vn + (1 - this->friction) * vt - this->ball->GetVelocity();
					glm::vec3 deltaVprim = velCol / (u * u + v * v);
					glm::vec3 v1New =this->velocitys[index1] + u * deltaVprim;
					glm::vec3 v2New =this->velocitys[index2] + v * deltaVprim;
				}
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
void Cloth::collision(int index, GLfloat timeStep, std::vector<Plain*> addtionalPlain) {

	//get and check next state
	std::vector<glm::vec3> result;
	if (this->isEular == true) {
		result = intEular(index, timeStep);
	}
	else {
		result = intRK(index, timeStep);
	}
	 
	glm::vec3 nextPos = result[0];
	glm::vec3 nextVel = result[1];
	//glm::vec3 acc = result[2];

	//detection
	std::vector<Plain*> hitPlains;
	std::vector<Plain*> temp;
	temp = plains;
	temp.insert(temp.end(), addtionalPlain.begin(), addtionalPlain.end());
	#pragma omp parallel
	#pragma omp for
	for (int i = 0; i < temp.size(); i++) {
		Plain* p = temp[i];
		if (p->checkHit(this->positions[index],nextPos,this->velocitys[index], this->radius).first) {
			hitPlains.push_back(p);
		}
	}

	//determination
	GLfloat f = 1;//give f a biggest value
	GLfloat restTime = 0.0f;
	//when there is at least one plain collide
	if (hitPlains.size() != 0) {
		std::cout << "hit"<<std::endl;
		Plain* hitPlain = hitPlains[0];
		//loop through all possible plains
		for (Plain* plain : hitPlains) {
			glm::vec3 p = plain->getOrigin();
			glm::vec3 nhat = plain->getNorm();
			//check signs
			GLfloat distN = glm::dot((positions[index] - p), nhat);
			GLfloat distNPONE = glm::dot((nextPos - p), nhat);

			glm::vec3 posCol = positions[index] - (1 + this->elastic)* distNPONE * nhat;
			glm::vec3 vn = glm::dot(nextVel, nhat) * nhat;
			glm::vec3 vt = nextVel - vn;
			glm::vec3 velCol = -this->elastic * vn + (1 - this->friction) * vt;

			this->positions[index] = posCol + hitPlain->getNorm() * 0.1f;
			this->velocitys[index] = velCol;
			this->forces[index] = defaultForces();
		}
	}
	else {
		this->positions[index] = nextPos;
		this->velocitys[index] = nextVel;
		this->forces[index] = defaultForces();
	}
	//renderUpdate();
}
////////////////////////////////////////////////////////////////////////////////
glm::vec3 Cloth::defaultForces() 
{
	return this->gravForce; //-this->airResist * this->velocity + this->airResist * this->windVelocity;
}
////////////////////////////////////////////////////////////////////////////////
void Cloth::renderUpdate() 
{
	if (!positions.empty()) {

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

	}
}
////////////////////////////////////////////////////////////////////////////////
glm::vec3 Cloth::ComputeSpringForce(std::tuple<int, int, GLfloat> spring) {

	//calculate spring force
	glm::vec3 p1CurPos = positions[std::get<0>(spring)];
	glm::vec3 p2CurPos = positions[std::get<1>(spring)];
	//
	GLfloat originLength = std::get<2>(spring);
	
	GLfloat currLength = glm::length(p2CurPos - p1CurPos);//calculate currLength
	if (isnan(currLength)) { currLength = 0; }

	//Hooke's law 
	glm::vec3 disPlcVec = (originLength - currLength) * glm::normalize(p2CurPos - p1CurPos);
	glm::vec3 fSpring = -(springConst)*disPlcVec;

	//caculating damper force
	glm::vec3 p1CurVec = velocitys[std::get<0>(spring)];//collcet particle velocity
	glm::vec3 p2CurVec = velocitys[std::get<1>(spring)];
	float vClose = glm::dot(p1CurVec - p2CurVec, glm::normalize(p2CurPos - p1CurPos));
	glm::vec3 fDamper = -dampingConst * vClose * glm::normalize(p2CurPos - p1CurPos);

	//apply force
	glm::vec3 force = fSpring + fDamper;
	return force;
	//this->forces[std::get<1>(spring)] = this->forces[std::get<1>(spring)] - force;
	//glm::vec3 acc = force / mass;
	//accelerations[std::get<0>(spring)] = accelerations[std::get<1>(spring)] + acc;
	//accelerations[std::get<1>(spring)] = accelerations[std::get<1>(spring)] + acc;

}
////////////////////////////////////////////////////////////////////////////////
glm::vec3 Cloth::ComputeSpringHelper(glm::vec3 pos1, glm::vec3 pos2, 
	glm::vec3 vel1, glm::vec3 vel2, GLfloat restLength) {

	//calculate spring force
	glm::vec3 p1CurPos = pos1;
	glm::vec3 p2CurPos = pos2;
	//
	GLfloat originLength = restLength;

	GLfloat currLength = glm::length(p2CurPos - p1CurPos);//calculate currLength
	if (isnan(currLength)) { currLength = 0; }

	//Hooke's law 
	glm::vec3 disPlcVec = (originLength - currLength) * glm::normalize(p2CurPos - p1CurPos);
	glm::vec3 fSpring = -(springConst)*disPlcVec;

	//caculating damper force
	glm::vec3 p1CurVec = vel1;//collcet particle velocity
	glm::vec3 p2CurVec = vel2;
	float vClose = glm::dot(p1CurVec - p2CurVec, glm::normalize(p2CurPos - p1CurPos));
	glm::vec3 fDamper = -dampingConst * vClose * glm::normalize(p2CurPos - p1CurPos);

	//apply force
	glm::vec3 force = fSpring + fDamper;
	return force;
	//this->forces[std::get<1>(spring)] = this->forces[std::get<1>(spring)] - force;
	//glm::vec3 acc = force / mass;
	//accelerations[std::get<0>(spring)] = accelerations[std::get<1>(spring)] + acc;
	//accelerations[std::get<1>(spring)] = accelerations[std::get<1>(spring)] + acc;

}
////////////////////////////////////////////////////////////////////////////////
void Cloth::calNorm(glm::vec3 windVel, std::tuple<int, int, int> triangle) {
	//get index

	int index1 = std::get<0>(triangle);
	int index2 = std::get<1>(triangle);
	int index3 = std::get<2>(triangle);

	//get positions
	glm::vec3 p1Pos = positions[index1];
	glm::vec3 p2Pos = positions[index2];
	glm::vec3 p3Pos = positions[index3];

	//calculate normal for triangle
	glm::vec3 n = glm::cross(p2Pos - p1Pos, p3Pos - p1Pos);
	n = glm::normalize(n);
	glm::vec3 averageN = glm::vec3( n.x/3.0, n.y / 3.0, n.z / 3.0 );

	this->normals[index1] = glm::normalize(this->normals[index1] + averageN);
	this->normals[index2] = glm::normalize(this->normals[index2] + averageN);
	this->normals[index3] = glm::normalize(this->normals[index3] + averageN);

	//caculation surface velocity
	glm::vec3 vSurface = (velocitys[index1] + velocitys[index2] + velocitys[index3]) / 3.0f;
	glm::vec3 v = vSurface - windVel;
	if (glm::length(v) < 1e-3) {
		return;
	}
	//calculating area
	GLfloat area = 0.5f * glm::length(glm::cross(p2Pos - p1Pos, p3Pos - p1Pos));
	area = area * glm::abs(glm::dot(glm::normalize(windVel), n));
	//caculating aero force 
	glm::vec3 fd = - dragCo * area * v;
	glm::vec3 crossProd = glm::cross(n, v);
	glm::vec3 fl = glm::vec3(0.0f);
	if (glm::length(crossProd) != 0) {
		 fl = -liftCo * area * (glm::cross(v, glm::normalize(glm::cross(n, v))));
	}
	
	glm::vec3 faero = fd + fl;
	
	// caculate propagated force
	GLfloat protion1 = abs(getDegree(p2Pos - p1Pos, p3Pos - p1Pos) / 180.0f);
	GLfloat protion2 = abs(getDegree(p1Pos - p2Pos, p3Pos - p2Pos) / 180.0f);
	GLfloat protion3 = 1 - protion1 - protion2;

	//applying aero force
	this->forces[index1] = this->forces[index1] + protion1 * faero;
	this->forces[index2] = this->forces[index2] + protion2 * faero;
	this->forces[index3] = this->forces[index3] + protion3 * faero;
}
////////////////////////////////////////////////////////////////////////////////
glm::vec3 Cloth::ComputeAeroHelper(glm::vec3 windVel, glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3,
	glm::vec3 vec1, glm::vec3 vec2, glm::vec3 vec3) {
	
	//get positions
	glm::vec3 p1Pos = pos1;
	glm::vec3 p2Pos = pos2;
	glm::vec3 p3Pos = pos3;

	//calculate normal for triangle
	glm::vec3 n = glm::cross(p2Pos - p1Pos, p3Pos - p1Pos);
	n = glm::normalize(n);
	glm::vec3 averageN = glm::vec3(n.x / 3.0, n.y / 3.0, n.z / 3.0);

	//caculation surface velocity
	glm::vec3 vSurface = (vec1 + vec2 + vec3) / 3.0f;
	glm::vec3 v = vSurface - windVel;
	if (glm::length(v) < 1e-3) {
		return glm::vec3(0);
	}
	//calculating area
	GLfloat area = 0.5f * glm::length(glm::cross(p2Pos - p1Pos, p3Pos - p1Pos));
	area = area * glm::abs(glm::dot(glm::normalize(windVel), n));
	//caculating aero force 
	glm::vec3 fd = -dragCo * area * v;
	glm::vec3 crossProd = glm::cross(n, v);
	glm::vec3 fl = glm::vec3(0.0f);
	if (glm::length(crossProd) != 0) {
		fl = -liftCo * area * (glm::cross(v, glm::normalize(glm::cross(n, v))));
	}

	glm::vec3 faero = fd + fl;

	return faero;
}
void Cloth::translate(GLfloat amount, glm::vec3 dir) {
	this->positions[0] = this->positions[0] + amount * dir;
	this->positions[pNum - 1] = this->positions[pNum - 1] + amount * dir;
}

////////////////////////////////////////////////////////////////////////////////
void Cloth::restoreDefault() 
{
	springConst = DEFAULT_SPRING_CONST;
	dampingConst = DEFAULT_DAMPING_CONST;
	dragCo = DEFAULT_DRAG_CONST;
	liftCo = DEFAULT_LIFT_CONST;
	elastic = DEFAULT_ELASTIC;

	grav = GRAV_COE;
	gravMult = DEFAULT_GRAVMULT;
	mass = DEFAULT_MASS;

	radius = DEFAULT_RADIUS;
	friction = DEFAULT_FRICTION;

	initPos = glm::vec3(0.0f, 20.0f, 0.0f);
	//velocity = glm::vec3(0.0f, -1.0f, 0.0f);
	windVelocity = glm::vec3(0.0f, 0.0f, -50.0f);

}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           