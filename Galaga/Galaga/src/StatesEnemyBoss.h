#pragma once

#include "EngineStates.h"

#include "ImageComponent.h"
#include "AnimationComponent.h"
#include "HealthComponent.h"
#include "HitboxComponent.h"

#include <queue>

namespace StatesEnemyBoss
{
	//------------------
	// Behavioral States
	//------------------
	class Spawning : public AE::FSMState
	{
	public:
		Spawning(std::queue<glm::vec2> seekPositions)
			: m_SeekPositions{ seekPositions } {}

		virtual void OnEnter(AE::GameObject* ) override {};
		virtual void OnExit(AE::GameObject* ) override {};
		virtual std::unique_ptr<AE::FSMState> Update(AE::GameObject* gameObject) override;

	private:

		std::queue<glm::vec2> m_SeekPositions{};
	};

	class Idle : public AE::FSMState
	{
	public:
		virtual void OnEnter(AE::GameObject* ) override {};
		virtual void OnExit(AE::GameObject* ) override {};
		virtual std::unique_ptr<AE::FSMState> Update(AE::GameObject* gameObject) override;

	private:

		float m_BombingRunChance{ 0.1f };  // chance to be chosen per second (1.f = 100%)
		float m_TractorBeamChance{ 0.2f }; // chance to be chosen per second (1.f = 100%)

	};
	class BombingRun : public AE::FSMState
	{
	public:
		virtual void OnEnter(AE::GameObject* gameObject) override;
		virtual void OnExit(AE::GameObject* ) override {};
		virtual std::unique_ptr<AE::FSMState> Update(AE::GameObject* gameObject) override;

	private:

		bool m_CanShoot{ true };
		float m_ShootHeight{ 125.f };

		float m_MoveSpeed{ 130.f };

		glm::vec2 m_SeekDir{};
		glm::vec2 m_SeekPos{ 0.f, 480.f };

	};
	class TractorBeam : public AE::FSMState
	{
	public:
		virtual void OnEnter(AE::GameObject* gameObject) override;
		virtual void OnExit(AE::GameObject* gameObject) override;
		virtual std::unique_ptr<AE::FSMState> Update(AE::GameObject* gameObject) override;

	private:

		float m_BeamDuration{3.f};
		float m_CurrentDuration{ 0.f };

		std::shared_ptr<HitboxComponent> m_BeamHitbox{};
		std::shared_ptr<AE::GameObject> m_BeamGO{};

		void SpawnBeam(AE::GameObject* go);

	};

	//---------------
	// Texture States
	//---------------
	class FullHealth : public AE::FSMState
	{
	public:
		virtual void OnEnter(AE::GameObject* gameObject) override;
		virtual void OnExit(AE::GameObject* ) override {};
		virtual std::unique_ptr<AE::FSMState> Update(AE::GameObject* gameObject) override;

	private:
		std::shared_ptr<HealthComponent> m_HealthComp{};
	};
	class HalfHealth : public AE::FSMState
	{
	public:
		virtual void OnEnter(AE::GameObject* gameObject) override;
		virtual void OnExit(AE::GameObject* ) override {};
		virtual std::unique_ptr<AE::FSMState> Update(AE::GameObject* gameObject) override;

	private:
	};
}
