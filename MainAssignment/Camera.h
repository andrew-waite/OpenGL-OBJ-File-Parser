#ifndef Camera_H
#define Camera_H

class Camera
{
	private:
		// angle of rotation for the camera direction
		float angle = 0.0f;

		// actual vector representing the camera's direction
		float lx = 0.0f, lz = -1.0f, ly = 1.0f;

		// XYZ position of the camera
		float cam_x = 0.0f, cam_z = 5.0f, cam_y = 1.0f;

		float mouseDirection = 0.0;

		// the key states. These variables will be zero
		//when no key is being presses
		float deltaAngle = 0.0f;
		float deltaMove = 0;
		int xOrigin = -1;

	public:
		Camera();
		float getAngle();
		float getLX();
		float getLY();
		float getLZ();
		float getCAMX();
		float getCAMY();
		float getCAMZ();
		float getMouseDirection();
		float getDeltaAngle();
		float getDeltaMove();
		int getXOrigin();

		void setAngle(float angle);
		void setLX(float lx);
		void setLY(float ly);
		void setLZ(float lz);
		void setCAMX(float camx);
		void setCAMY(float camy);
		void setCAMZ(float camz);
		void setMouseDirection(float mouseDirection);
		void setDeltaAngle(float deltaAngle);
		void setDeltaMove(float deltaMove);
		void setXOrigin(int xOrigin);

		void crossProduct(bool positive, float ax, float ay, float az, float bx, float by, float bz);
		void computePos(float deltaMove);
};

#endif