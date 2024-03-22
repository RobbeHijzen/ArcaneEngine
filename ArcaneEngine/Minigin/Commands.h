#pragma once

#include <memory>

#include "Time.h"
#include "GameObject.h"

namespace Commands
{
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
}


