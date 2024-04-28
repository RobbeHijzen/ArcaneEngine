#include "RotatingComponent.h"
#include "MyTime.h"

constexpr float PI_F = 3.14159265f;

RotatingComponent::RotatingComponent(GameObject* const parentGameObject, float radius, float rotationSpeed)
	: BaseComponent(parentGameObject)
	, m_Radius{radius}
	, m_RotationSpeed{rotationSpeed}
{
}

void RotatingComponent::Update()
{
	m_CurrentAngle += Time::GetInstance().GetDeltaTime() * m_RotationSpeed;
	while (m_CurrentAngle >= 2 * PI_F) m_CurrentAngle -= 2 * PI_F;


	float xPos{cosf(m_CurrentAngle) * m_Radius};
	float yPos{sinf(m_CurrentAngle) * m_Radius};

	GetOwner()->SetLocalTransform({ xPos, yPos });
}
