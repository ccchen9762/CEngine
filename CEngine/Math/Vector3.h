#pragma once

#include <cmath>

struct Vector3 {
public:
	Vector3() = default;
	Vector3(float x, float y, float z=0.0f);
	~Vector3() = default;
	
	float length() { return sqrtf(m_x*m_x + m_y*m_y + m_z*m_z); }
	void normalize();

	float m_x = 0.0f;
	float m_y = 0.0f;
	float m_z = 0.0f;
};

struct Vector3Int {
public:
	Vector3Int() = default;
	Vector3Int(int x, int y, int z = 0);
	~Vector3Int() = default;

	int m_x = 0;
	int m_y = 0;
	int m_z = 0;
};