#include "StatesEnemyBoss.h"


//****
// Idle
//****
void StatesEnemyBoss::Idle::OnEnter(AE::GameObject* )
{
}
void StatesEnemyBoss::Idle::OnExit(AE::GameObject* )
{
}
AE::FSMState* StatesEnemyBoss::Idle::Update(AE::GameObject* )
{
	return nullptr;
}

//****
// BombingRun
//****
void StatesEnemyBoss::BombingRun::OnEnter(AE::GameObject* )
{
}
void StatesEnemyBoss::BombingRun::OnExit(AE::GameObject* )
{
}
AE::FSMState* StatesEnemyBoss::BombingRun::Update(AE::GameObject* )
{
	return nullptr;
}

//****
// TractorBeamSetup
//****
void StatesEnemyBoss::TractorBeamSetup::OnEnter(AE::GameObject* )
{
}
void StatesEnemyBoss::TractorBeamSetup::OnExit(AE::GameObject* )
{
}
AE::FSMState* StatesEnemyBoss::TractorBeamSetup::Update(AE::GameObject* )
{
	return nullptr;
}

//****
// TractorBeam
//****
void StatesEnemyBoss::TractorBeam::OnEnter(AE::GameObject* )
{
}
void StatesEnemyBoss::TractorBeam::OnExit(AE::GameObject* )
{
}
AE::FSMState* StatesEnemyBoss::TractorBeam::Update(AE::GameObject* )
{
	return nullptr;
}

//****
// TwoHealth
//****
void StatesEnemyBoss::FullHealth::OnEnter(AE::GameObject* gameObject)
{
	if (auto imageComp = gameObject->GetComponent<ImageComponent>())
	{
		imageComp->SetSourcePos(1.f, 91.f);
	}

	m_HealthComp = gameObject->GetComponent<HealthComponent>();
}
void StatesEnemyBoss::FullHealth::OnExit(AE::GameObject* )
{
}
AE::FSMState* StatesEnemyBoss::FullHealth::Update(AE::GameObject*)
{
	if (m_HealthComp)
	{
		if (m_HealthComp->GetHealth() <= 1)
		{
			return new HalfHealth();
		}
	}
	return nullptr;
}

//****
// OneHealth
//****
void StatesEnemyBoss::HalfHealth::OnEnter(AE::GameObject* gameObject)
{
	if (auto imageComp = gameObject->GetComponent<ImageComponent>())
	{
		imageComp->SetSourcePos(1.f, 127.f);
	}
}
void StatesEnemyBoss::HalfHealth::OnExit(AE::GameObject* )
{
}
AE::FSMState* StatesEnemyBoss::HalfHealth::Update(AE::GameObject* )
{
	return nullptr;
}
