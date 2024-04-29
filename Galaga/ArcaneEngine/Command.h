#pragma once

#include "GameObject.h"

namespace AE
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


}


