#pragma once

#include "FSMState.h"

#include "ImageComponent.h"
#include "AnimationComponent.h"
#include "HealthComponent.h"
#include "HitboxComponent.h"
#include "EnemySpawningStructs.h"
#include "MovementImageComponent.h"

#include <queue>

namespace StatesEnemyButterfly
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
		float m_MaxShootHeight{250.f};
		int m_BombingChance{50};
		
		glm::vec2 m_IdlePos{};

		float m_VerticalDistance{70.f};
		float m_HorizontalDistance{100.f};

		float m_ShootDelay{ 0.5f };
		int m_ShootAmount{ 2 };

	};
}
