#include "RotatingComponent.h"
#include "Time.h"

constexpr float PI_F = 3.14159265f;

RotatingComponent::RotatingComponent(GameObject* const parentGameObject, glm::vec2 center, float radius, float rotationSpeed)
	: BaseComponent(parentGameObject)
	, m_Center{center}
	, m_Radius{radius}
	, m_RotationSpeed{rotationSpeed}
{
}

void RotatingComponent::Update()
{
	m_CurrentAngle += Time::GetInstance().GetDeltaTime() * m_RotationSpeed;
	while (m_CurrentAngle >= 2 * PI_F) m_CurrentAngle -= 2 * PI_F;


	float xPos{cosf(m_CurrentAngle) * m_Radius + m_Center.x};
	float yPos{sinf(m_CurrentAngle) * m_Radius + m_Center.y};

	GetOwner()->SetLocalTransform({ xPos, yPos });
}
