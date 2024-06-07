#pragma once

#include "FSMState.h"

#include "ImageComponent.h"
#include "AnimationComponent.h"
#include "HealthComponent.h"
#include "HitboxComponent.h"
#include "EnemySpawningStructs.h"
#include "MovementImageComponent.h"
#include "MovementComponent.h"
#include "TimeManager.h"

#include <queue>

namespace StatesEnemyBoss
{
	//------------------
	// Behavioral States
	//------------------
	
	class Idle : public AE::FSMState
	{
	public:
		Idle(glm::vec2 startingPos) : m_StartingPos{ startingPos } {}

		virtual void OnEnter(AE::GameObject* ) override {};
		virtual void OnExit(AE::GameObject* ) override {};
		virtual std::unique_ptr<AE::FSMState> Update(AE::GameObject* gameObject) override;

		void ChangeToBombingRun() { m_ChangeToBombingRun = true; }
		void ChangeToTractorBeam() { m_ChangeToTractorBeam = true; }

	private:
		bool m_ChangeToBombingRun{ false };
		bool m_ChangeToTractorBeam{ false };

		glm::vec2 m_StartingPos{};

	};
	class BombingRun : public AE::FSMState
	{
	public:
		BombingRun(glm::vec2 idlePos) : m_IdlePos{ idlePos } {}

		virtual void OnEnter(AE::GameObject* gameObject) override;
		virtual void OnExit(AE::GameObject* ) override {};
		virtual std::unique_ptr<AE::FSMState> Update(AE::GameObject* gameObject) override;

	private:

		bool m_CanShoot{ true };
		float m_ShootHeight{ 240.f };
		float m_RandomHeightOffset{50.f};

		float m_MoveSpeed{ 90.f };

		glm::vec2 m_SeekDir{};
		glm::vec2 m_SeekPos{ 0.f, 480.f };

		glm::vec2 m_IdlePos{};

		float m_ShootDelay{ 0.5f };
		int m_ShootAmount{ 2 };


	};
	class TractorBeam : public AE::FSMState
	{
	public:
		TractorBeam(glm::vec2 idlePos) : m_IdlePos{ idlePos } {}

		virtual void OnEnter(AE::GameObject* gameObject) override;
		virtual void OnExit(AE::GameObject* ) override {};
		virtual std::unique_ptr<AE::FSMState> Update(AE::GameObject* gameObject) override;

		void SwitchToBeamSuck(AE::GameObject* go)
		{
			m_SwitchToBeamSuck = true; 
			m_PlayerGO = go;
			if (auto moveComp = go->GetComponent<MovementComponent>())
			{
				moveComp->SetCanMove(false);
				AE::TimeManager::GetInstance().SetTimer([moveComp](int) { moveComp->SetCanMove(true); }, m_PlayerMovementLockoutDuration, 1);
			}
		}

	private:

		float m_BeamDuration{3.f};
		float m_CurrentDuration{ 0.f };

		float m_PlayerMovementLockoutDuration{ 5.f };

		std::shared_ptr<AE::GameObject> m_BeamGO{};

		bool m_SwitchToBeamSuck{ false };
		AE::GameObject* m_PlayerGO{};

		void SpawnBeam(AE::GameObject* go);

		glm::vec2 m_IdlePos{};
	};
	
	class BeamSuck : public AE::FSMState
	{
	public:
		BeamSuck(glm::vec2 idlePos, AE::GameObject* player, std::shared_ptr<AE::GameObject> beamObj)
			: m_IdlePos{ idlePos } 
			, m_PlayerGO{player}
			, m_BeamGO{beamObj}
		{}

		virtual void OnEnter(AE::GameObject* ) override {};
		virtual void OnExit(AE::GameObject*) override {};
		virtual std::unique_ptr<AE::FSMState> Update(AE::GameObject* gameObject) override;

		void SpawnRedGalaga(AE::GameObject* enemyGO);

	private:
		float m_SuckSpeed{25.f};

		AE::GameObject* m_PlayerGO{};
		std::shared_ptr<AE::GameObject> m_BeamGO{};
		glm::vec2 m_IdlePos{};

		bool m_MovePlayer{ true };
		bool m_ChangeState{ false };

		float m_AFKTimeAfterKill{1.6f};
	};

	//---------------
	// Texture States
	//---------------
	class FullHealth : public AE::FSMState
	{
	public:
		FullHealth(glm::vec2 sourcePos) : m_SourcePos{ sourcePos } {}

		virtual void OnEnter(AE::GameObject* gameObject) override;
		virtual void OnExit(AE::GameObject* ) override {};
		virtual std::unique_ptr<AE::FSMState> Update(AE::GameObject* gameObject) override;

	private:
		std::shared_ptr<HealthComponent> m_HealthComp{};
		glm::vec2 m_SourcePos{};
	};
	class HalfHealth : public AE::FSMState
	{
	public:
		HalfHealth(glm::vec2 sourcePos) : m_SourcePos{ sourcePos } {}

		virtual void OnEnter(AE::GameObject* gameObject) override;
		virtual void OnExit(AE::GameObject* ) override {};
		virtual std::unique_ptr<AE::FSMState> Update(AE::GameObject* gameObject) override;

	private:
		glm::vec2 m_SourcePos{};
	};
}
