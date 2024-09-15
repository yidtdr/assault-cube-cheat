#pragma once
#include <algorithm>

constexpr float DEG2RAD = 3.14159265359f / 180.0f;

class Vector3 {
public:

	float x, y, z;

	Vector3() {};
	Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}
	Vector3 operator + (const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
	Vector3 operator - (const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
	Vector3 operator * (const float& rhs) const { return Vector3(x * rhs, y * rhs, z * rhs); }
	Vector3 operator / (const float& rhs) const { return Vector3(x / rhs, y / rhs, z / rhs); }
	Vector3& operator += (const Vector3& rhs) { return *this = *this + rhs; }
	Vector3& operator -= (const Vector3& rhs) { return *this = *this - rhs; }
	Vector3& operator *= (const float& rhs) { return *this = *this * rhs; }
	Vector3& operator /= (const float& rhs) { return *this = *this / rhs; }

	float Length() const { return sqrtf(x * x + y * y + z * z); }
	Vector3 Normalize() const { return *this * (1 / Length()); }
	float Distance(const Vector3& rhs) const { return (*this - rhs).Length(); }
};

struct Vector4
{
	float x, y, z, w;
};

struct Vector2
{
	float x, y;
};

struct Matrix
{
	float matrix[16];
};

inline float v2Dist(Vector2 a, Vector2 b)
{
	return sqrtf(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)));
}

float DegreesToRadians(float num);
float RadiansToDegrees(float num);
Vector2 DegreesToRadians(Vector2 vec);
Vector2 RadiansToDegrees(const Vector2 vec);
Vector2 CalcAngle(Vector3 origin, Vector3 target);


inline float DegreesToRadians(float num) {
	return num / 180.0f * 3.14159265f;
}

inline float RadiansToDegrees(float num) {
	return num / 3.14159265f * 180.0f;
}

inline Vector2 DegreesToRadians(Vector2 vec) {
	return Vector2{ DegreesToRadians(vec.x), DegreesToRadians(vec.y) };
}

inline Vector2 RadiansToDegrees(Vector2 vec) {
	return Vector2{ RadiansToDegrees(vec.x), RadiansToDegrees(vec.y) };
}

inline Vector2 CalcAngle(Vector3 origin, Vector3 target)
{
	Vector2 results{ 0.0f, 0.0f };
	results.x = RadiansToDegrees(-atan2f(target.x - origin.x, target.y - origin.y)) - 180;
	if (results.x <= 0.0f)
	{
		results.x += 360.0f;
	}

	results.y = RadiansToDegrees(asinf((target.z - origin.z) / origin.Distance(target)));
	return results;
}

inline Vector3 WorldToScreen(Vector3& pos, float matrix[16], int windowWidth, int windowHeight)
{
	Vector4 clipCoords;

	clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
	clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
	clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
	clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

	// Check if the point is behind the camera
	if (clipCoords.w < 0.1f)
	{
		return Vector3{ 0.0f, 0.0f, 0.0f }; // Object is not visible
	}

	// Perspective division to normalize the coordinates
	Vector3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	// Convert NDC to screen space
	Vector3 screen;
	screen.x = (windowWidth / 2.0 * NDC.x) + (NDC.x + windowWidth / 2.0);
	screen.y = -(windowHeight / 2.0 * NDC.y) + (NDC.y + windowHeight / 2.0);
	screen.z = NDC.z;

	return screen;
}