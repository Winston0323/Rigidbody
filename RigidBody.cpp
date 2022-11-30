#include "RigidBody.h"


RigidBody::RigidBody(std::string objFilename, GLfloat pointSize)
{
	
	objParser(objFilename);//setup render data
	restoreDefault();//setup constance
	moveToWorldCenter();//correct render data
	
	// Set the model matrix to an identity matrix. 
	this->gravForce = glm::vec3(0, -this->totalMass * this->grav * this->gravMult, 0);
	this->forces = defaultVertexForce();
	//this->forces[28] = glm::vec3(0, 1000, 0);
	//this->forces[20] = glm::vec3(1000, 0, 0);
	//this->forces[27] = glm::vec3(0, 1000, 0);
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

	//generate ebo 
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * faces.size(), faces.data(), GL_STATIC_DRAW);


	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

RigidBody::~RigidBody()
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &VBOvertex);
	glDeleteBuffers(1, &VBOnormal);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);
}

void RigidBody::draw(const glm::mat4& viewProjMtx, GLuint shader)
{
	// actiavte the shader program 
	glUseProgram(shader);
	// get the locations and send the uniforms to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
	glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);
	glUniform4fv(glGetUniformLocation(shader, "astColor"), 1, glm::value_ptr(this->color));

	// Bind the VAO
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

	// draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, faces.size()* 3, GL_UNSIGNED_INT, 0);
	
	//// Set point size
	//glPointSize(1);
	//// Draw the points 
	//glDrawArrays(GL_POINTS, 0, points.size());
	
	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void RigidBody::update(GLfloat deltaTime)
{
	//spin(0.1f);
	this->gravForce = glm::vec3(0, -this->totalMass * this->grav * this->gravMult, 0);
	collision(deltaTime);
	updateThreeDegree();
}

void RigidBody::update()
{
}

void RigidBody::spin(float angle, glm::vec3 axis)
{
	// Update the model matrix by multiplying a rotation matrix
	model = glm::rotate(glm::radians(angle), axis) * model;

}
void RigidBody::spinPerVetex(float angle, glm::vec3 axis)
{
	// Update the model matrix by multiplying a rotation matrix
	for (int i = 0; i < points.size(); i++) {
		this->points[i] = glm::vec3(glm::rotate(glm::radians(angle), axis) * glm::vec4(this->points[i], 1.0f));
	}
}
void RigidBody::scale(float level)
{
	for (int i = 0; i < points.size(); i++) {
		this->points[i].x = this->points[i].x * level;
		this->points[i].y = this->points[i].y * level;
		this->points[i].z = this->points[i].z * level;
	}
}
void RigidBody::translation(glm::vec3 destination)
{
	//move the masss center to the destination point
	this->massCenter= glm::vec3(destination);
	model = glm::translate(glm::mat4(1.0f), destination);
}
void RigidBody::translationPerVertex(glm::vec3 destination)
{
	this->massCenter = glm::vec3(destination);
	//model = glm::translate(glm::mat4(1.0f), destination);
	//for (int i = 0; i < points.size(); i++) {
	//	this->points[i] = glm::vec3(glm::translate(glm::mat4( 1.0f), destination) * glm::vec4(this->[i], 1.0f));
	//}
}
////////////////////////////////////////////////////////////////////////////////
std::vector<glm::vec3> RigidBody::intEular(GLfloat timeStep) {
	
	//Linear momentum
	glm::vec3 acc = this->frcOnMassCtr / this->totalMass;//get and check next state
	glm::vec3 nextPMon = this->PMom + this->frcOnMassCtr * timeStep;
	//calculate x(n+1)
	glm::vec3 nextPos = this->massCenter + this->velocity * timeStep;
	glm::vec3 nextVel = this->PMom / this->totalMass;
	this->frcOnMassCtr = defaultForce();
	std::vector<glm::vec3> result;
	result.push_back(nextPos);
	result.push_back(nextVel);
	result.push_back(nextPMon);
	result.push_back(acc);
	return result;
}
////////////////////////////////////////////////////////////////////////////////
std::vector<glm::vec3> RigidBody::intRK( GLfloat timeStep) {
	//get and check next state
	//glm::vec3 K1Acc = this->frcOnMassCtr / this->totalMass;//get and check next state
	
	//calculate x(n+1)
	glm::vec3 K1PMon = this->PMom + this->frcOnMassCtr * timeStep;
	glm::vec3 K1Pos = this->massCenter + this->velocity * timeStep;
	glm::vec3 K1Vec = K1PMon / this->totalMass;

	//loop through all springs that the index is connected
	glm::vec3 K2force = defaultForce();
	glm::vec3 K2PMon = this->PMom + K2force * timeStep;
	glm::vec3 K2Pos = this->massCenter + K1Vec * timeStep;
	glm::vec3 K2Vec = K2PMon/ this->totalMass;

	//loop through all springs that the index is connected
	glm::vec3 K3force = defaultForce();
	glm::vec3 K3PMon = this->PMom + K3force * 0.5f * timeStep;
	glm::vec3 K3Pos = this->massCenter + K2Vec * 0.5f * timeStep;
	glm::vec3 K3Vec = K3PMon / this->totalMass;

	//loop through all springs that the index is connected
	//loop through all springs that the index is connected
	glm::vec3 K4force = defaultForce();
	glm::vec3 K4PMon = this->PMom + K4force * timeStep;
	glm::vec3 K4Pos = this->massCenter + K3Vec * timeStep;
	glm::vec3 K4Vec = K4PMon / this->totalMass;

	glm::vec3 nextPos = this->massCenter + timeStep * K2Vec;

	glm::vec3 nextPMon = this->PMom + timeStep * K2force;

	glm::vec3 nextVel = nextPMon / this->totalMass;

	std::vector<glm::vec3> result;
	result.push_back(nextPos);
	result.push_back(nextVel);
	result.push_back(nextPMon);
	return result;
}
////////////////////////////////////////////////////////////////////////////////
glm::vec3 RigidBody::defaultForce()
{
	return this->gravForce; //-this->airResist * this->velocity + this->airResist * this->windVelocity;
}
////////////////////////////////////////////////////////////////////////////////
std::vector<glm::vec3> RigidBody::defaultVertexForce()
{
	std::vector<glm::vec3> result;
	for (int i = 0; i < points.size(); i++) {
		result.push_back(this->gravForce); //-this->airResist * this->velocity + this->airResist * this->windVelocity;
	}
	return result;
}

//void RigidBody::edgeCollision(GLfloat timeStep) {
//	//get all edges
//	for (std::pair<int, int> currEdge : edges) {
//		int index1 = currEdge.first;//first vertex index
//		int index2 = currEdge.second;//second vertex index
//
//		//edge
//		glm::vec3 Q1 = this->points[index1] + this->rotMat * this->localPos[index1];
//		glm::vec3 Q2 = this->points[index2] + this->rotMat * this->localPos[index2];
//		glm::vec3 b = Q2 - Q1;
//
//		//updated egde
//		std::vector<glm::vec3> result1 = intEular(timeStep);
//		//calcuate quaternion
//		glm::quat rotQuad = glm::quat_cast(this->rotMat);//R
//		glm::quat angVelQuat = glm::quat(0, this->angVel.x, this->angVel.y, this->angVel.z);//w
//		glm::quat nextRotQuat = rotQuad + timeStep * 1.0f / 2.0f * angVelQuat * rotQuad;//R + h *1/2*w
//		glm::mat3 nextRot = glm::mat3_cast(glm::normalize(nextRotQuat));
//		
//		glm::vec3 Q1New = result1[0] + nextRot * this->localPos[index1];
//		glm::vec3 Q1VelNew = result1[1];
//
//		std::vector<glm::vec3> result2 = result1;
//		glm::vec3 Q2New = result2[0] + nextRot * this->localPos[index1];
//		glm::vec3 Q2VelNew = result2[1];
//		glm::vec3 bNew = Q2New - Q1New;
//
//		//get obsticle edges
//		for (std::pair<glm::vec3, glm::vec3> edgepair : this->obtedges) {
//			//obsticle edges
//			glm::vec3 P1 = edgepair.first;
//			glm::vec3 P2 = edgepair.second;
//			glm::vec3 a = P2 - Q1;
//
//			//nhat between two edges
//			if (glm::dot(a, b) <= EPSILON) {
//				continue;
//			}
//
//			glm::vec3 nhat = glm::normalize(glm::cross(a, b));
//
//			glm::vec3 r = Q1 - P1;
//			glm::vec3 rNew = Q1New - P1;
//
//			//point on obsticle edge
//			GLfloat s = glm::dot(r, glm::cross(glm::normalize(b), nhat)) /
//				glm::dot(a, glm::cross(glm::normalize(b), nhat));
//
//			//point on colliding edge
//			GLfloat t = glm::dot(-r, glm::cross(glm::normalize(a), nhat)) /
//				glm::dot(b, glm::cross(glm::normalize(a), nhat));
//			//updated collding edge
//			GLfloat tNew = glm::dot(-r, glm::cross(glm::normalize(a), nhat)) /
//				glm::dot(bNew, glm::cross(glm::normalize(a), nhat));
//			if (s < 0 || s > 1 || t < 0 || t > 1) {
//				continue;
//			}
//			else {
//				//calculate point on edges
//				glm::vec3 pa = P1 + s * a;
//				glm::vec3 qa = Q1 + t * b;
//				//updated point
//				glm::vec3 qaNew = Q1New + tNew * bNew;
//				//calculate m 
//				glm::vec3 mOld = qa - pa;
//				glm::vec3 mNew = qaNew - pa;
//
//				if (glm::length(mOld) == TOLERANCE || glm::dot(mOld, mNew) < 0) {
//					GLfloat lenghtP = glm::length(P2 - P1);
//					GLfloat lenghtP1 = glm::length(pa - P1);
//					GLfloat u = lenghtP1 / lenghtP;
//					GLfloat v = 1.0f - u;
//					glm::vec3 nextVel = Q1VelNew * u + Q2VelNew * v;
//					glm::vec3 posCol = pa;
//					glm::vec3 vn = glm::dot(nextVel, nhat) * nhat;
//					glm::vec3 vt = nextVel - vn;
//					glm::vec3 velCol = -this->elastic * vn + (1 - this->friction) * vt - this->ball->GetVelocity();
//					glm::vec3 deltaVprim = velCol / (u * u + v * v);
//					glm::vec3 v1New = this->velocitys[index1] + u * deltaVprim;
//					glm::vec3 v2New = this->velocitys[index2] + v * deltaVprim;
//				}
//			}
//		}
//	}
//}
////////////////////////////////////////////////////////////////////////////////
void RigidBody::collision(GLfloat timeStep) {
	//clear the forces
	
	this->forces.clear();
	//get and check next state
	
	std::vector<glm::vec3> result = intRK(timeStep);

	glm::vec3 nextPos = result[0];
	glm::vec3 nextVel = result[1];
	glm::vec3 nextPMom = result[2];
	
	//calculate torque
	glm::vec3 torque = glm::vec3(0);
	for (int i = 0; i < this->forces.size(); i++) {
		glm::vec3 r = this->points[i] - this->massCenter;
		glm::vec3 currTrq = glm::cross(r, forces[i]);
		torque = torque + currTrq;
	}
	//update angualar momentum and velocity
	glm::vec3 nextLMom = this->LMom + timeStep * torque;//update angular momentmum
	glm::vec3 nextAngVel = this->iInverse * this->LMom;//use angular momentum calculate angular velocity

	//calcuate quaternion
	glm::quat rotQuad = glm::quat_cast(this->rotMat);//R
	glm::quat angVelQuat = glm::quat(0, this->angVel.x, this->angVel.y, this->angVel.z);//w
	glm::quat nextRotQuat = rotQuad + timeStep * 1.0f / 2.0f * angVelQuat * rotQuad;//R + h *1/2*w
	glm::mat3 nextRot = glm::mat3_cast(glm::normalize(nextRotQuat));
	glm::mat3 nextIInv = nextRot * (this->iInvInit * glm::transpose(nextRot));//I inverse

	////calcuate quaternion
	//glm::quat angVelQuat = glm::quat(0, this->angVel.x, this->angVel.y, this->angVel.z);//w
	//glm::quat nextRotQuat = rotQuat + timeStep * 1.0f / 2.0f * angVelQuat * rotQuat;//R + h *1/2*w
	//glm::mat3 nextRot = glm::mat3_cast(nextRotQuat);


	//get the local to world points
	std::vector<glm::vec3> nextPoses = localToWorld(nextPos, this->rotMat); //p1 = p0 + rpi

	//detection
	Plain* hitPlain;
	GLfloat f = 1.0f/DEFAULT_SIMRATE;//give f a biggest value

	bool everHit = false;
	
	for (int i = 0; i < points.size(); i++) {//loop through all points
		bool hit = false;
		int hitplain = 0;
		for (int j = 0; j < this->colliders.size(); j++) {//loop through all plains
			Plain* p = this->colliders[j];
			if (p->checkHit(this->points[i], nextPoses[i], this->velocity, 1,i).first) {
				if (p->checkHit(this->points[i], nextPoses[i], this->velocity, 1, i).second < f)
				{
					hitPlain = p;
					hitPlain->setLastHit(i);
					hit = true;
					everHit = true;
					f = p->checkHit(this->points[i], nextPoses[i], this->velocity, 1, i).second;
					nextPMom = this->PMom;
					nextLMom = this->LMom;
					nextIInv = this->iInverse;
					nextAngVel = this->angVel;
					nextPos = this->massCenter;
					nextPoses = this->points;
					nextRot = this->rotMat;
					hitplain = j;

				}
				else {
					p->setLastHit(-1);
				
				}
				
			}
			else {
				p->setLastHit(-1);
			}
		}
		glm::vec3 ra = this->points[i] - this->massCenter;
		if (hit) {//at least one hit
			printf("Point%d %f, %f, %f hit plain %d \n", i,points[i].x, points[i].y, points[i].z, hitplain);
			
			glm::vec3 p = hitPlain->getOrigin();
			glm::vec3 nhat = hitPlain->getNorm();

			//glm::vec3 hitPoint = this->massCenter + this->velocity * f;
			//glm::quat rotQuad = glm::quat_cast(this->rotMat);//R
			//glm::quat angVelQuat = glm::quat(0, this->angVel.x, this->angVel.y, this->angVel.z);//w
			//glm::quat nextRotQuat = rotQuad + f * 1.0f / 2.0f * angVelQuat * rotQuad;//R + h *1/2*w
			//glm::mat3 tempRot = glm::mat3_cast(nextRotQuat);
			//hitPoint = hitPoint + tempRot * this->localPos[i];//hitpoint
			
			glm::vec3 paderv = this->velocity + glm::cross(this->angVel, ra);
			GLfloat vOld = glm::dot(paderv, nhat);//v-
			std::cout << "V- is "<< vOld<< endl;
			
			//calculate j
			GLfloat dividend = -1 * (1 + this->elastic) * vOld;//−(1 + cr)v−
			
			glm::vec3 insideCrossLeft = glm::cross(ra, nhat);//Iinv(ra × nhat)
			glm::vec3 crossProduct = this->iInverse * glm::cross(insideCrossLeft, ra);		//Iinv(ra × nhat) × ra
			GLfloat divisor = (1.0f / this->totalMass) + dot(nhat, crossProduct);
			GLfloat j = dividend / divisor;
			
			glm::vec3 deltaP = j * nhat;
			glm::vec3 deltaL = j * glm::cross(ra, nhat);
			//overwrite the velocities and momentums into after collision 
			nextPMom = nextPMom + deltaP;
			nextLMom = nextLMom + deltaL;
			nextVel = nextVel + nextPMom / this->totalMass;
			nextAngVel = nextAngVel + this->iInverse * deltaL;
			glm::vec3 panew = nextVel + glm::cross(nextAngVel, ra);
			GLfloat vnew = glm::dot(panew, nhat);
			//if (glm::abs(vnew) > glm::abs(vOld)) {
			//	nextPMom = nextPMom - deltaP;
			//	nextLMom = nextLMom - deltaL;
			//
			//}
			std::cout << "V+ is " << vnew << endl;
			//std::cout << "V+ is " << vnew.x << " " << vnew.y << " " << vnew.z << endl;
			//this->massCenter = this->massCenter + EPSILON * nhat;
			//break;
		}

	}
	//update the state vector
	//linear
	this->massCenter = nextPos;		//c.o.m.
	this->points = nextPoses;		//pi
	this->velocity = nextVel;		//v
	this->PMom = nextPMom;			//P
	
	//angular
	this->rotMat = nextRot;			//R
	this->angVel = nextAngVel;		//w
	this->iInverse = nextIInv;		//I-1
	this->LMom = nextLMom;			//L
	
	


}

void RigidBody::objParser(string objFilename)
{
	std::ifstream objFile(objFilename); // The obj file we are reading.
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<glm::ivec3> faces;
	
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
			if (label == "f")
			{
				glm::ivec3 face;
				string x, y, z;
				int xCord, yCord, zCord;
				ss >> x >> y >> z;
				xCord = stoi(x.substr(0, x.find("//"))) - 1;
				yCord = stoi(y.substr(0, y.find("//"))) - 1;
				zCord = stoi(z.substr(0, z.find("//"))) - 1;
				face.x = xCord;
				face.y = yCord;
				face.z = zCord;
				faces.push_back(face);
				//std::pair<int,int> edge1 = std::make_pair(face.x, face.y);
				//std::pair<int,int> edge2 = std::make_pair(face.x, face.z);
				//std::pair<int,int> edge3 = std::make_pair(face.y, face.z);
				//edges.push_back(edge1);
				//edges.push_back(edge2);
				//edges.push_back(edge3);
			}
		}

		//update points normals and faces
		this->points = points;
		this->normals = normals;
		this->faces = faces;
	}
	else
	{
		std::cerr << "Can't open the file " << objFilename << std::endl;
	}
	objFile.close();
}
void RigidBody::moveToWorldCenter() {
	std::vector<GLfloat> xCord;
	std::vector<GLfloat> yCord;
	std::vector<GLfloat> zCord;
	for (int i = 0; i < points.size(); i++) {
		xCord.push_back(this->points[i].x);
		yCord.push_back(this->points[i].y);
		zCord.push_back(this->points[i].z);
	}

	GLfloat midX = (*std::max_element(xCord.begin(), xCord.end()) + *std::min_element(xCord.begin(), xCord.end())) / 2;
	GLfloat midY = (*std::max_element(yCord.begin(), yCord.end()) + *std::min_element(yCord.begin(), yCord.end())) / 2;
	GLfloat midZ = (*std::max_element(zCord.begin(), zCord.end()) + *std::min_element(zCord.begin(), zCord.end())) / 2;
	//initialize
	
	for (int i = 0; i < points.size(); i++) {

		points[i].x = points[i].x - midX;
		points[i].y = points[i].y - midY;
		points[i].z = points[i].z - midZ;
	}
	scale(1.0);

	//update mass center
	GLfloat volumn = 0;
	glm::vec3 weightedPos = glm::vec3(0);
	for (int i = 0; i <	faces.size(); i++) {
		//massSumPos = massSumPos + this->mass * this->points[i];
		glm::vec3 d = this->points[faces[i].x];
		glm::vec3 e = this->points[faces[i].y];
		glm::vec3 f = this->points[faces[i].z];
		GLfloat vtemp = (1.0f / 6.0f) * glm::dot(d, glm::cross(e, f));
		glm::vec3 xtemp = (1.0f / 4.0f) * (d + e + f);
		weightedPos = weightedPos + vtemp * xtemp;
		volumn = volumn + vtemp;
	}

	this->massCenter = weightedPos / volumn;
	this->massCenterRef = this->massCenter;

	for (int i = 0; i < points.size(); i++) {

		points[i].x = points[i].x - this->massCenter.x;
		points[i].y = points[i].y - this->massCenter.y;
		points[i].z = points[i].z - this->massCenter.z;
	}
	this->totalMass = this->pointMass * this->points.size();
	this->geoCenter = glm::vec3(0, 0, 0);
	std::vector<GLfloat> xCord1;
	std::vector<GLfloat> yCord1;
	std::vector<GLfloat> zCord1;
	for (int i = 0; i < points.size(); i++) {
		xCord1.push_back(this->points[i].x);
		yCord1.push_back(this->points[i].y);
		zCord1.push_back(this->points[i].z);
	}
	//get the three size of the rigid body
	this->xMax = *std::max_element(xCord1.begin(), xCord1.end());
	this->xMin = *std::min_element(xCord1.begin(), xCord1.end());
	this->yMax = *std::max_element(yCord1.begin(), yCord1.end());
	this->yMin = *std::min_element(yCord1.begin(), yCord1.end());
	this->zMax = *std::max_element(zCord1.begin(), zCord1.end());
	this->zMin = *std::min_element(zCord1.begin(), zCord1.end());

	glm::mat3 inertia = glm::mat3(0);
	//get the local coodinate from mass center
	for (int i = 0; i < points.size(); i++) {

		//glm::vec3 localCord = glm::vec3(points[i].x - this->massCenter.x,
		//	points[i].y - this->massCenter.y, points[i].z - this->massCenter.z);
		glm::vec3 localCord = points[i];
		this->localPos.push_back(localCord);
		
		//first column
		inertia[0].x = inertia[0].x + (localCord.y * localCord.y + localCord.z * localCord.z) * this->pointMass;	// (y2 + z2)mi
		inertia[0].y = inertia[0].y -  localCord.x * localCord.y * this->pointMass;								//-(x * y)
		inertia[0].z = inertia[0].z -  localCord.x * localCord.z * this->pointMass;								//-(x * z)
		
		//second column
		inertia[1].x = inertia[1].x -  localCord.x * localCord.y * this->pointMass;								//-(x * y)
		inertia[1].y = inertia[1].y + (localCord.x * localCord.x + localCord.z * localCord.z) * this->pointMass;	// (x2 + z2)mi
		inertia[1].z = inertia[1].z -  localCord.y * localCord.z * this->pointMass;								//-(y * z)

		//third column
		inertia[2].x = inertia[2].x -  localCord.x * localCord.z * this->pointMass;								//-(x * z)
		inertia[2].y = inertia[2].y -  localCord.y * localCord.z * this->pointMass;								//-(y * z)
		inertia[2].z = inertia[2].z + (localCord.x * localCord.x + localCord.y * localCord.y) * this->pointMass;	// (x2 + y2)mi
		
	}
	this->massCenter = glm::vec3(0);
	this->inertia = inertia;
	this->inertiaInit = inertia;
	this->iInverse = glm::inverse(inertia);
	this->iInvInit = glm::inverse(inertia);
}

std::vector<glm::vec3> RigidBody::localToWorld(glm::vec3 massCenter, glm::mat3 rotMat) {

	std::vector<glm::vec3> worldPos;
	for (int i = 0; i < points.size(); i++) {
		 worldPos.push_back(massCenter + rotMat * this->localPos[i]);
	}

	return worldPos;
}
void RigidBody::updateThreeDegree() {
	std::vector<GLfloat> xCord1;
	std::vector<GLfloat> yCord1;
	std::vector<GLfloat> zCord1;
	for (int i = 0; i < points.size(); i++) {
		xCord1.push_back(this->points[i].x);
		yCord1.push_back(this->points[i].y);
		zCord1.push_back(this->points[i].z);
	}
	//get the three size of the rigid body
	this->xMax = *std::max_element(xCord1.begin(), xCord1.end());
	this->xMin = *std::min_element(xCord1.begin(), xCord1.end());
	this->yMax = *std::max_element(yCord1.begin(), yCord1.end());
	this->yMin = *std::min_element(yCord1.begin(), yCord1.end());
	this->zMax = *std::max_element(zCord1.begin(), zCord1.end());
	this->zMin = *std::min_element(zCord1.begin(), zCord1.end());
}
void RigidBody::restoreDefault() {
	this->color = glm::vec3(0.3, 0.3, 0.4);
	this->pointMass = 0.1f;
	this->model = glm::mat4(1);
	this->rotMat = glm::mat3(1);
	//this->rotMat[0] = glm::vec3(0,-1,0);
	//this->rotMat[1] = glm::vec3(1,0,0);
	//this->rotMat[2] = glm::vec3(0,0,1);
	this->iInverse = this->iInvInit;
	this->massCenter = massCenterRef;
	this->angVel = glm::vec3(0,0,0);
	this->LMom = glm::vec3(0,0,0);
	this->PMom = glm::vec3(0);
	this->elastic = 0.1f;
	this->grav = 9.8f;
	this->gravMult = 0.0f;
	this->frcOnMassCtr = glm::vec3(0);
	this->velocity = glm::vec3(0);
}
