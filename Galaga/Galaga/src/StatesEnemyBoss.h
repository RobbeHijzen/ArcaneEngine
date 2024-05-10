#pragma once

#include "FSMState.h"
#include "GameObject.h"

#include "ImageComponent.h"
#include "AnimationComponent.h"
#include "HealthComponent.h"
#include "HitboxComponent.h"


namespace StatesEnemyBoss
{
	//------------------
	// Behavioral States
	//------------------
	class Return : public AE::FSMState
	{
	public:
		virtual void OnEnter(AE::GameObject* gameObject) override;
		virtual void OnExit(AE::GameObject* gameObject);
		virtual AE::FSMState* Update(AE::GameObject* gameObject);

	private:
		float m_MoveSpeed{ 130.f };

		glm::vec2 m_SeekDir{};
		glm::vec2 m_SeekPos{ 275.f, 50.f };

	};
	class Idle : public AE::FSMState
	{
	public:
		virtual void OnEnter(AE::GameObject* gameObject) override;
		virtual void OnExit(AE::GameObject* gameObject);
		virtual AE::FSMState* Update(AE::GameObject* gameObject);

	private:

		float m_BombingRunChance{ 0.1f };  // chance to be chosen (1.f = 100%), per second
		float m_TractorBeamChance{ 0.2f }; // chance to be chosen (1.f = 100%), per second

	};
	class BombingRun : public AE::FSMState
	{
	public:
		virtual void OnEnter(AE::GameObject* gameObject);
		virtual void OnExit(AE::GameObject* gameObject);
		virtual AE::FSMState* Update(AE::GameObject* gameObject);

	private:

		bool m_CanShoot{ true };
		float m_ShootHeight{ 125.f };

		float m_MoveSpeed{ 130.f };

		glm::vec2 m_SeekDir{};
		glm::vec2 m_SeekPos{ 0.f, 480.f };

		void ShootBullet();

	};
	class TractorBeamSetup : public AE::FSMState
	{
	public:
		virtual void OnEnter(AE::GameObject* gameObject);
		virtual void OnExit(AE::GameObject* gameObject);
		virtual AE::FSMState* Update(AE::GameObject* gameObject);

	private:

		float m_MoveSpeed{ 130.f };
		float m_StopHeight{ 300.f };

		glm::vec2 m_SeekDir{};
		glm::vec2 m_SeekPos{};

	};
	class TractorBeam : public AE::FSMState
	{
	public:
		virtual void OnEnter(AE::GameObject* gameObject);
		virtual void OnExit(AE::GameObject* gameObject);
		virtual AE::FSMState* Update(AE::GameObject* gameObject);

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
		virtual void OnEnter(AE::GameObject* gameObject);
		virtual void OnExit(AE::GameObject* gameObject);
		virtual AE::FSMState* Update(AE::GameObject* gameObject);

	private:
		std::shared_ptr<HealthComponent> m_HealthComp{};
	};
	class HalfHealth : public AE::FSMState
	{
	public:
		virtual void OnEnter(AE::GameObject* gameObject);
		virtual void OnExit(AE::GameObject* gameObject);
		virtual AE::FSMState* Update(AE::GameObject* gameObject);

	private:
	};
}
