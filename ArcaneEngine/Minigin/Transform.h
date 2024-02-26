#pragma once
#include <glm/glm.hpp>


class Transform final
{
public:

	Transform(float x = 0.f, float y = 0.f, float z = 0.f) { m_Position = { x, y, z }; }

	const glm::vec3& GetPosition() const { return m_Position; }
	void SetPosition(float x, float y, float z);


	Transform operator+(Transform other)
	{
		glm::vec3 newPos{ m_Position + other.GetPosition() };

		return Transform{newPos.x, newPos.y, newPos.z};
	}
	Transform operator-(Transform other)
	{
		glm::vec3 newPos{ m_Position - other.GetPosition() };

		return Transform{ newPos.x, newPos.y, newPos.z };
	}

private:

	glm::vec3 m_Position;
};

