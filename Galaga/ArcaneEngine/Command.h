#pragma once

#include "GameObject.h"
#include <functional>

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
	class LambdaCommand final : public Command
	{
	public:
		LambdaCommand(std::function<void()> func) : m_Func{ func } {}
		virtual ~LambdaCommand() {}

		virtual void Execute() override 
		{
			m_Func();
		}
	private:
		std::function<void()> m_Func{};
	};


}


