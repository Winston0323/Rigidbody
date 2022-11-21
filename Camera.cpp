////////////////////////////////////////
// Camera.cpp
////////////////////////////////////////

#include "Camera.h"

#include "glm/gtx/euler_angles.hpp"

////////////////////////////////////////////////////////////////////////////////

Camera::Camera() {
	Reset();
}

////////////////////////////////////////////////////////////////////////////////

void Camera::Update() {
	// Compute camera world matrix
	glm::mat4 world(1);
	world[3][2] = Distance;
	world = glm::eulerAngleY(glm::radians(-Azimuth)) * glm::eulerAngleX(glm::radians(-Incline)) * world;
	
	this->camPos = world[3];
	
	// Compute view matrix (inverse of world matrix)
	View = glm::inverse(world);


	// Compute perspective projection matrix
	Project = glm::perspective(glm::radians(FOV), Aspect, NearClip, FarClip);

	// Compute final view-projection matrix
	ViewProjectMtx = Project * View;
}

////////////////////////////////////////////////////////////////////////////////

void Camera::Reset() {
	FOV = 45.0f;
	Aspect = 1.33f;
	NearClip = 0.1f;
	FarClip = 100.0f;

	Distance = 40.0f;
	Azimuth = 0.0f;
	Incline = 40.0f;
}

////////////////////////////////////////////////////////////////////////////////
