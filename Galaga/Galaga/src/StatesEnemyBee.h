#pragma once

#include "FSMState.h"

#include "ImageComponent.h"
#include "AnimationComponent.h"
#include "HealthComponent.h"
#include "HitboxComponent.h"
#include "EnemySpawningStructs.h"
#include "MovementImageComponent.h"

#include <queue>

namespace StatesEnemyBee
{
	//------------------
	// Behavioral States
	//------------------

	class Idle : public AE::FSMState
	{
	public:
		Idle(glm::vec2 startingPos) : m_StartingPos{ startingPos } {}

		virtual void OnEnter(AE::GameObject*) override {};
		virtual void OnExit(AE::GameObject*) override {};
		virtual std::unique_ptr<AE::FSMState> Update(AE::GameObject* gameObject) override;

		void ChangeToBombingRun() { m_ChangeToBombingRun = true; }

	private:
		bool m_ChangeToBombingRun{ false };

		glm::vec2 m_StartingPos{};

	};
	class BombingRun : public AE::FSMState
	{
	public:
		BombingRun(glm::vec2 idlePos) : m_IdlePos{ idlePos } {}

		virtual void OnEnter(AE::GameObject* gameObject) override;
		virtual void OnExit(AE::GameObject*) override {};
		virtual std::unique_ptr<AE::FSMState> Update(AE::GameObject* gameObject) override;
		
	private:
		// Stages
		glm::vec2 m_Stage1Offset{300.f, 15.f};
		float m_Stage2Height{430.f};
		float m_Stage3Height{465.f};
		float m_Stage4Height{430.f};

		glm::vec2 m_IdlePos{};
		
		float m_ShootDelay{ 0.5f };
		int m_ShootAmount{ 2 };

	};
}
