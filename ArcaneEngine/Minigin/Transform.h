#pragma once
#include <glm.hpp>

class Transform final
{
public:

	Transform(float x = 0.f, float y = 0.f, float z = 0.f) { m_Position = { x, y, z }; }
	Transform(glm::vec3 position) { m_Position = position; }
	Transform(glm::vec2 positionXY, float positionZ = 0.f) { m_Position = {positionXY, positionZ}; }

	const glm::vec3& GetPosition() const { return m_Position; }
	void SetPosition(float x, float y, float z);


	Transform operator+(Transform other)
	{
		glm::vec3 newPos{ m_Position + other.GetPosition() };

		return Transform{ newPos };
	}
	Transform operator+=(Transform other)
	{
		m_Position += other.GetPosition();

		return Transform{ m_Position };
	}

	Transform operator-(Transform other)
	{
		glm::vec3 newPos{ m_Position - other.GetPosition() };

		return Transform{ newPos };
	}
	Transform operator-=(Transform other)
	{
		m_Position -= other.GetPosition();

		return Transform{ m_Position };
	}

private:

	glm::vec3 m_Position;
};

