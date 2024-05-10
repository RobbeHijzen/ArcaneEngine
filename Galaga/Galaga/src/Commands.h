#pragma once

#include "ArcaneEngine.h"

#include "Time.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "ShootComponent.h"

#include "SceneManager.h"

class MoveCommand : public AE::GameObjectCommand
{
public:
	MoveCommand(AE::GameObject* gameObject, glm::vec2 direction, float movementSpeed)
		: GameObjectCommand(gameObject)
		, m_MovementSpeed{ movementSpeed }
	{
		assert(std::abs(direction.x) > 0 || std::abs(direction.y) > 0);
		m_Direction = glm::normalize(direction);
	}

	virtual void Execute() override
	{
		glm::vec2 addedPosition{ m_Direction * m_MovementSpeed * AE::Time::GetInstance().GetDeltaTime() };
		GetGameObject()->AddLocalTransform(AE::Transform{ addedPosition });
	}

private:
	glm::vec2 m_Direction;
	float m_MovementSpeed;
};


class ShootCommand : public AE::GameObjectCommand
{
public:

	ShootCommand(AE::GameObject* gameObject)
		: GameObjectCommand(gameObject)
	{
		m_ShootComp = gameObject->GetComponent<ShootComponent>();
	}

	virtual void Execute() override
	{
		if (m_ShootComp)
		{
			m_ShootComp->FireBullet();
		}
	}

private:
	std::shared_ptr<ShootComponent> m_ShootComp{};

};

class LoadCommand : public AE::GameObjectCommand
{
public:

	LoadCommand(AE::GameObject* gameObject)
		: GameObjectCommand(gameObject)
	{}

	virtual void Execute() override
	{
		AE::SceneManager::GetInstance().IncrementScene();
	}
};

