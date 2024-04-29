#pragma once

#include "ArcaneEngine.h"

#include "Time.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"

using namespace EngineCommands;

class MoveCommand : public GameObjectCommand
{
public:
	MoveCommand(GameObject* gameObject, glm::vec2 direction, float movementSpeed)
		: GameObjectCommand(gameObject)
		, m_MovementSpeed{ movementSpeed }
	{
		assert(std::abs(direction.x) > 0 || std::abs(direction.y) > 0);
		m_Direction = glm::normalize(direction);
	}

	virtual void Execute() override
	{
		glm::vec2 addedPosition{ m_Direction * m_MovementSpeed * Time::GetInstance().GetDeltaTime() };
		GetGameObject()->AddLocalTransform(Transform{ addedPosition });
	}

private:
	glm::vec2 m_Direction;
	float m_MovementSpeed;
};

class KillCommand : public GameObjectCommand
{
public:

	KillCommand(GameObject* gameObject) : GameObjectCommand(gameObject) {}

	virtual void Execute() override
	{
		GetGameObject()->GetComponent<HealthComponent>()->KillObject();
	}
};

class PickupSilverCommand : public GameObjectCommand
{
public:

	PickupSilverCommand(GameObject* gameObject)
		: GameObjectCommand(gameObject)
	{}

	virtual void Execute() override
	{
		GetGameObject()->NotifyAll(ObserverPattern::Event::SilverPickup);
	}
};
class PickupGoldCommand : public GameObjectCommand
{
public:

	PickupGoldCommand(GameObject* gameObject)
		: GameObjectCommand(gameObject)
	{}

	virtual void Execute() override
	{
		GetGameObject()->NotifyAll(ObserverPattern::Event::GoldPickup);
	}
};

class ShootCommand : public GameObjectCommand
{
public:

	ShootCommand(GameObject* gameObject)
		: GameObjectCommand(gameObject)
	{}

	virtual void Execute() override
	{
		GetGameObject()->NotifyAll(ObserverPattern::Event::BulletFired);
	}
};

