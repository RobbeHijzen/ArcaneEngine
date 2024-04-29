#pragma once

#include "ArcaneEngine.h"

#include "Time.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"

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

class KillCommand : public AE::GameObjectCommand
{
public:

	KillCommand(AE::GameObject* gameObject) : GameObjectCommand(gameObject) {}

	virtual void Execute() override
	{
		GetGameObject()->GetComponent<HealthComponent>()->KillObject();
	}
};

class PickupSilverCommand : public AE::GameObjectCommand
{
public:

	PickupSilverCommand(AE::GameObject* gameObject)
		: GameObjectCommand(gameObject)
	{}

	virtual void Execute() override
	{
		GetGameObject()->NotifyAll(AE::Event::SilverPickup);
	}
};
class PickupGoldCommand : public AE::GameObjectCommand
{
public:

	PickupGoldCommand(AE::GameObject* gameObject)
		: GameObjectCommand(gameObject)
	{}

	virtual void Execute() override
	{
		GetGameObject()->NotifyAll(AE::Event::GoldPickup);
	}
};

class ShootCommand : public AE::GameObjectCommand
{
public:

	ShootCommand(AE::GameObject* gameObject)
		: GameObjectCommand(gameObject)
	{}

	virtual void Execute() override
	{
		GetGameObject()->NotifyAll(AE::Event::BulletFired);
	}
};

