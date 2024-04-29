#include "ProjectileMovementComponent.h"

#include "MyTime.h"

void ProjectileMovementComponent::Update()
{
	float deltaTime{ AE::Time::GetInstance().GetDeltaTime() };

	MoveObject(deltaTime);
	UpdateLifeTime(deltaTime);
}

void ProjectileMovementComponent::MoveObject(float deltaTime)
{
	float xDistance{ m_Direction.x * m_Speed * deltaTime };
	float yDistance{ m_Direction.y * m_Speed * deltaTime };

	GetOwner()->AddLocalTransform(AE::Transform{ {xDistance, yDistance} });
}

void ProjectileMovementComponent::UpdateLifeTime(float deltaTime)
{
	m_LifeTime -= deltaTime;
	if (m_LifeTime <= 0.f)
	{
		GetOwner()->Delete();
	}
}

