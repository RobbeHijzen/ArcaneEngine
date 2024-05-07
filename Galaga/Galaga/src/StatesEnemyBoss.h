#pragma once

#include "FSMState.h"
#include "GameObject.h"

#include "ImageComponent.h"
#include "HealthComponent.h"


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

	};
	class TractorBeam : public AE::FSMState
	{
	public:
		virtual void OnEnter(AE::GameObject* gameObject);
		virtual void OnExit(AE::GameObject* gameObject);
		virtual AE::FSMState* Update(AE::GameObject* gameObject);

	private:

	};

	//---------------
	// Texture States
	//---------------
	class TwoHealth : public AE::FSMState
	{
	public:
		virtual void OnEnter(AE::GameObject* gameObject);
		virtual void OnExit(AE::GameObject* gameObject);
		virtual AE::FSMState* Update(AE::GameObject* gameObject);

	private:
		std::shared_ptr<HealthComponent> m_HealthComp{};
	};
	class OneHealth : public AE::FSMState
	{
	public:
		virtual void OnEnter(AE::GameObject* gameObject);
		virtual void OnExit(AE::GameObject* gameObject);
		virtual AE::FSMState* Update(AE::GameObject* gameObject);

	private:
	};
}
