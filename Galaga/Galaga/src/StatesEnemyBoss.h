#pragma once

#include "FSMState.h"
#include "GameObject.h"

#include "ImageComponent.h"
#include "HealthComponent.h"
#include "HitboxComponent.h"


namespace StatesEnemyBoss
{
	//------------------
	// Behavioral States
	//------------------
	class Idle : public AE::FSMState
	{
	public:
		virtual void OnEnter(AE::GameObject* gameObject) override;
		virtual void OnExit(AE::GameObject* gameObject);
		virtual AE::FSMState* Update(AE::GameObject* gameObject);

	private:

		float m_BombingRunChance{ 0.04f };  // chance to be chosen (1.f = 100%), per second
		float m_TractorBeamChance{ 0.04f }; // chance to be chosen (1.f = 100%), per second

	};
	class BombingRun : public AE::FSMState
	{
	public:
		virtual void OnEnter(AE::GameObject* gameObject);
		virtual void OnExit(AE::GameObject* gameObject);
		virtual AE::FSMState* Update(AE::GameObject* gameObject);

	private:

	};
	class TractorBeamSetup : public AE::FSMState
	{
	public:
		virtual void OnEnter(AE::GameObject* gameObject);
		virtual void OnExit(AE::GameObject* gameObject);
		virtual AE::FSMState* Update(AE::GameObject* gameObject);

	private:

		float m_MoveSpeed{ 100.f };
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

		float m_BeamWidth{30.f};

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
