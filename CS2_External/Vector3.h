#pragma once

class Vector3 {
public:
	float x, y, z;
	Vector3();
	Vector3(float x, float y, float z);
	float lengthSquare();
	float length();
	bool isNull();
	inline void Normalize()
	{
		while (y < -180)
			y += 360;
		while (y > 180)
			y -= 360;
		if (x > 89)
			x = 89;
		if (x < -89)
			x = -89;
	}
	inline void clamp() {
		if (x < -89.0f) {
			x = -89.0f;
		}

		if (x > 89.0f) {
			x = 89.0f;
		}

		while (y < -180.0f) {
			y += 360.0f;
		}
		while (y > 180.0f) {
			y -= 360.0f;
		}

		z = 0.0f;
	}
	Vector3 operator+(Vector3 v);
	Vector3 operator-(Vector3 v);
	Vector3 operator*(float f);
};