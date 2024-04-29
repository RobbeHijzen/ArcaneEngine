#pragma once

#include "BaseComponent.h"
#include "GameObject.h"

class ProjectileMovementComponent : public BaseComponent
{
public:
	ProjectileMovementComponent(GameObject* pParent, glm::vec2 direction, float speed, float lifeTime = 0.f)
		: BaseComponent(pParent)
		, m_Speed{speed}
		, m_LifeTime{lifeTime}
	{
		m_Direction = glm::normalize(direction);

		if(lifeTime == 0.f) m_HasLifeTime = false;
	}

	virtual void Update() override;

private:

	float m_Speed;
	glm::vec2 m_Direction;

	bool m_HasLifeTime{ true };
	float m_LifeTime{};

	void MoveObject(float deltaTime);
	void UpdateLifeTime(float deltaTime);
};

