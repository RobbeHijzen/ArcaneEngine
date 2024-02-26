#pragma once
#include <glm/glm.hpp>


class Transform final
{
public:

	Transform(float x = 0.f, float y = 0.f, float z = 0.f) { m_Position = { x, y, z }; }

	const glm::vec3& GetPosition() const { return m_Position; }
	void SetPosition(float x, float y, float z);


	const glm::vec3& operator+(glm::vec3 other)
	{
		return m_Position + other;
	}

private:

	glm::vec3 m_Position;
};

