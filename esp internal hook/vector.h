#pragma once
#include <math.h> // https://docs.microsoft.com/en-us/windows/win32/api/tlhelp32/

struct Vec3 {
	float x, y, z;

	Vec3 operator+(Vec3 d) {
		return { x + d.x, y + d.y, z + d.z };
	}
	Vec3 operator/(float d) {
		return { x / d, y / d, z / d };
	}
	Vec3 operator-(Vec3 d) {
		return { x - d.x, y - d.y, z - d.z };
	}
	Vec3 operator*(float d) {
		return { x * d, y * d, z * d };
	}

	void Normalize() {
		while (y < -180) { y += 360; }
		while (y > 180) { y -= 360; }
		if (x > 89) { x = 89; }
		if (x < -89) { x = -89; }
	}
	float dot() const { return x * x + y * y + z * z; }
	float Length() const { return sqrtf(dot()); }
};

struct Vec2 {
	float x, y;

	Vec2 operator+(Vec2 d) {
		return { x + d.x, y + d.y };
	}
	Vec2 operator-(Vec2 d) {
		return { x - d.x, y - d.y };
	}
	Vec2 operator*(float d) {
		return { x * d, y * d };
	}

	void Normalize() {
		while (y < -180) { y += 360; }
		while (y > 180) { y -= 360; }
		if (x > 89) { x = 89; }
		if (x < -89) { x = -89; }
	}
};

struct Vec4 {
	float x, y, z, w;
};