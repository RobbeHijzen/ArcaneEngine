#include "StatesEnemyBoss.h"

#include "MyTime.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Observers.h"
#include "ShootComponent.h"

//****
// Helper Functions
//****
static glm::vec2 GetSeekDirection(glm::vec2 seekPos, AE::GameObject* gameObject)
{
	glm::vec2 dir{ seekPos - glm::vec2{gameObject->GetLocalTransform().GetPosition()} };

	if (dir.x || dir.y)
	{
		return glm::normalize(dir);
	}
	return glm::vec2{};
}
static bool HasReached(glm::vec2 seekPos, AE::GameObject* gameObject, float acceptanceRadius = 1.f)
{
	glm::vec2 difference{ seekPos - glm::vec2{gameObject->GetLocalTransform().GetPosition()} };
	float distanceSq{glm::sqrt(difference.x * difference.x + difference.y * difference.y)};

	return distanceSq <= acceptanceRadius * acceptanceRadius;
}


//****
// Return
//****
void StatesEnemyBoss::Return::OnEnter(AE::GameObject* gameObject)
{
	m_SeekDir = GetSeekDirection(m_SeekPos, gameObject);
}
void StatesEnemyBoss::Return::OnExit(AE::GameObject* )
{
}
AE::FSMState* StatesEnemyBoss::Return::Update(AE::GameObject* gameObject)
{
	glm::vec2 addedPosition{ m_SeekDir * m_MoveSpeed * AE::Time::GetInstance().GetDeltaTime() };
	addedPosition.y = std::abs(addedPosition.y); // so the enemy will move downwards

	gameObject->AddLocalTransform(AE::Transform{ addedPosition });

	if (gameObject->GetWorldTransform().GetPosition().y > 480.f)
	{
		gameObject->AddLocalTransform(AE::Transform{ 0.f, -580.f });
		m_SeekDir = GetSeekDirection(m_SeekPos, gameObject);
	}

	if (HasReached(m_SeekPos, gameObject))
	{
		return new Idle();
	}
	return nullptr;
}

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
	float dt{AE::Time::GetInstance().GetDeltaTime()};

	auto randomnum{ (float) rand() / RAND_MAX};
	if (randomnum <= m_BombingRunChance * dt)
	{
		return new BombingRun();
	}
	randomnum = (float)rand() / RAND_MAX;
	if (randomnum <= m_TractorBeamChance * dt)
	{
		return new BombingRun();
	}
	return nullptr;
}

//****
// BombingRun
//****
void StatesEnemyBoss::BombingRun::OnEnter(AE::GameObject* gameObject)
{
	m_SeekPos.x = rand() % 2 ? 0.f : 640.f;
	m_SeekDir = GetSeekDirection(m_SeekPos, gameObject);
}
void StatesEnemyBoss::BombingRun::OnExit(AE::GameObject* )
{
}
AE::FSMState* StatesEnemyBoss::BombingRun::Update(AE::GameObject* gameObject)
{
	glm::vec2 addedPosition{ m_SeekDir * m_MoveSpeed * AE::Time::GetInstance().GetDeltaTime() };
	gameObject->AddLocalTransform(AE::Transform{ addedPosition });

	float yHeight{ gameObject->GetWorldTransform().GetPosition().y };
	if (m_CanShoot && yHeight >= m_ShootHeight)
	{
		m_CanShoot = false;
		if (auto shootComp = gameObject->GetComponent<ShootComponent>())
		{
			shootComp->FireBullet();
		}
	}

	if (yHeight > 480.f) return new Return();
	return nullptr;
}

void StatesEnemyBoss::BombingRun::ShootBullet()
{
}

//****
// TractorBeamSetup
//****
void StatesEnemyBoss::TractorBeamSetup::OnEnter(AE::GameObject* gameObject)
{
	float seekXValue{float(rand() % 540 + 50)};
	m_SeekPos = glm::vec2{ seekXValue, m_StopHeight };

	m_SeekDir = GetSeekDirection(m_SeekPos, gameObject);
}
void StatesEnemyBoss::TractorBeamSetup::OnExit(AE::GameObject* )
{
}
AE::FSMState* StatesEnemyBoss::TractorBeamSetup::Update(AE::GameObject* gameObject)
{
	glm::vec2 addedPosition{ m_SeekDir * m_MoveSpeed * AE::Time::GetInstance().GetDeltaTime() };
	gameObject->AddLocalTransform(AE::Transform{ addedPosition });

	if (HasReached(m_SeekPos, gameObject))
	{
		return new TractorBeam();
	}
	return nullptr;
}

//****
// TractorBeam
//****
void StatesEnemyBoss::TractorBeam::OnEnter(AE::GameObject* gameObject)
{
	SpawnBeam(gameObject);
}
void StatesEnemyBoss::TractorBeam::OnExit(AE::GameObject* )
{
	m_BeamGO->Delete();
}
AE::FSMState* StatesEnemyBoss::TractorBeam::Update(AE::GameObject*)
{
	m_CurrentDuration += AE::Time::GetInstance().GetDeltaTime();

	if (m_CurrentDuration >= m_BeamDuration)
	{
		if (m_BeamHitbox)
		{
			m_BeamHitbox->SetActive(true);
			return new Return();
		}
	}

	return nullptr;
}
void StatesEnemyBoss::TractorBeam::SpawnBeam(AE::GameObject* go)
{
	m_BeamGO = std::make_shared<AE::GameObject>();
	m_BeamGO->SetLocalTransform(go->GetWorldTransform());
	m_BeamGO->AddLocalTransform(AE::Transform{ -20.f, 40.f });
	m_BeamGO->AddObserver(std::move(std::make_unique<BeamObserver>()));

	auto beamHitbox{ std::make_shared<HitboxComponent>(m_BeamGO.get(), 80.f, 140.f) };
	beamHitbox->SetActive(false);
	beamHitbox->AddIgnoreTag("Enemy");
	m_BeamHitbox = beamHitbox;

	m_BeamGO->AddComponent(beamHitbox);

	// Image
	auto imageComp{ std::make_shared<ImageComponent>(m_BeamGO.get(), "TractorBeam.png") };
	imageComp->SetDestRect(80.f, 140.f);
	imageComp->SetSourceRect(816, 0, 48, 80);
	m_BeamGO->AddComponent(imageComp);

	AE::SceneManager::GetInstance().GetCurrentScene()->Add(m_BeamGO);
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
