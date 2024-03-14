#pragma once

#include <memory>

#include "GameObject.h"
#include "MovementComponent.h"

class Command
{
public:
	virtual ~Command() {}
	virtual void Execute() = 0;

};
class GameObjectCommand : public Command
{
public:
	GameObjectCommand(GameObject* gameObject) : m_pGameObject{ gameObject } {}
	virtual ~GameObjectCommand() {}

protected:
	GameObject* GetGameObject() const { return m_pGameObject; }

private:
	GameObject* m_pGameObject{};
};


class MoveCommand : public GameObjectCommand
{
public:
	MoveCommand(GameObject* gameObject, glm::vec2 direction) : GameObjectCommand(gameObject), m_Direction{ direction.x, -direction.y } {}

	virtual void Execute() override 
	{
		if (auto movementComp{ GetGameObject()->GetComponent<MovementComponent>() })
		{
			movementComp->Move(m_Direction);
		}
	}

private:
	glm::vec2 m_Direction;
};


