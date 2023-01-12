#include "Vector3.h"

Vector3::Vector3(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {
}

void Vector3::normalize() {
	float len = length();
	m_x /= len, m_y /= len, m_z /= len;
}

Vector3Int::Vector3Int(int x, int y, int z) : m_x(x), m_y(y), m_z(z) {
}
