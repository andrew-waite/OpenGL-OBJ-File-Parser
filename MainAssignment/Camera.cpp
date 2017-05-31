#include "Camera.h"

Camera::Camera() { }

void Camera::computePos(float deltaMove) 
{

	cam_x += deltaMove * lx * 1.0f;
	cam_z += deltaMove * lz * 1.0f;
	cam_y += deltaMove * ly * 1.0f;
}

void Camera::crossProduct(bool positive, float ax, float ay, float az, float bx, float by, float bz)
{
	float strafeSpeed = 0.2;
	float cx = 0.0f, cy, cz;
	cx = ay * bz - az * by;
	cy = az * bx - ax * bz;
	cz = ax * by - ay * bx;

	cx * strafeSpeed;
	cy * strafeSpeed;
	cz * strafeSpeed;

	if (positive)
	{
		cam_x += cx;
		cam_y += cy;
		cam_z += cz;
	}
	else
	{
		cam_x -= cx;
		cam_y -= cy;
		cam_z -= cz;
	}
}

float Camera::getAngle()
{
	return angle;
}

float Camera::getLX()
{
	return lx;
}

float Camera::getLY()
{
	return ly;
}

float Camera::getLZ()
{
	return lz;
}

float Camera::getCAMX()
{
	return cam_x;
}

float Camera::getCAMY()
{
	return cam_y;
}

float Camera::getCAMZ()
{
	return cam_z;
}

float Camera::getMouseDirection()
{
	return mouseDirection;
}

float Camera::getDeltaAngle()
{
	return deltaAngle;
}

float Camera::getDeltaMove()
{
	return deltaMove;
}

int Camera::getXOrigin()
{
	return xOrigin;
}

void Camera::setAngle(float angle)
{
	this->angle = angle;
}

void Camera::setLX(float lx)
{
	this->lx = lx;
}

void Camera::setLY(float ly)
{
	this->ly = ly;
}

void Camera::setLZ(float lz)
{
	this->lz = lz;
}

void Camera::setCAMX(float camx)
{
	this->cam_x = camx;
}

void Camera::setCAMY(float camy)
{
	this->cam_y = camy;
}

void Camera::setCAMZ(float camz)
{
	this->cam_z = camz;
}

void Camera::setMouseDirection(float mouseDirection)
{
	this->mouseDirection = mouseDirection;
}

void Camera::setDeltaAngle(float deltaAngle)
{
	this->deltaAngle = deltaAngle;
}

void Camera::setDeltaMove(float deltaMove)
{
	this->deltaMove = deltaMove;
}

void Camera::setXOrigin(int xOrigin)
{
	this->xOrigin = xOrigin;
}