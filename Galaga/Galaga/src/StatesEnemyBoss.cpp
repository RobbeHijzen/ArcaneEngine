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
void StatesEnemyBoss::TwoHealth::OnEnter(AE::GameObject* gameObject)
{
	if (auto imageComp = gameObject->GetComponent<ImageComponent>())
	{
		imageComp->SetSourcePos(1.f, 91.f);
	}

	m_HealthComp = gameObject->GetComponent<HealthComponent>();
}
void StatesEnemyBoss::TwoHealth::OnExit(AE::GameObject* )
{
}
AE::FSMState* StatesEnemyBoss::TwoHealth::Update(AE::GameObject*)
{
	if (m_HealthComp)
	{
		if (m_HealthComp->GetHealth() <= 1)
		{
			return new OneHealth();
		}
	}
	return nullptr;
}

//****
// OneHealth
//****
void StatesEnemyBoss::OneHealth::OnEnter(AE::GameObject* gameObject)
{
	if (auto imageComp = gameObject->GetComponent<ImageComponent>())
	{
		imageComp->SetSourcePos(1.f, 127.f);
	}
}
void StatesEnemyBoss::OneHealth::OnExit(AE::GameObject* )
{
}
AE::FSMState* StatesEnemyBoss::OneHealth::Update(AE::GameObject* )
{
	return nullptr;
}