#include "StatesEnemyBoss.h"

#include "MyTime.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Observers.h"
#include "ShootComponent.h"

//****
// Spawning
//****
std::unique_ptr<AE::FSMState> StatesEnemyBoss::Spawning::Update(AE::GameObject* )
{
	if (m_SeekPositions.size() > 0)
	{
		glm::vec2 nextSeekPos{ m_SeekPositions.front() };
		m_SeekPositions.pop();

		auto nextState{ std::make_unique<Spawning>(m_SeekPositions) };
		return std::move(std::make_unique<AE::States::Seek>(std::move(nextState), nextSeekPos));
	}

	return std::move(std::make_unique<Idle>());
}

//****
// Idle
//****
std::unique_ptr<AE::FSMState> StatesEnemyBoss::Idle::Update(AE::GameObject* )
{
	float dt{AE::Time::GetInstance().GetDeltaTime()};

	auto randomnum{ (float) rand() / RAND_MAX};
	if (randomnum <= m_BombingRunChance * dt)
	{
		return std::move(std::make_unique<BombingRun>());
	}
	randomnum = (float)rand() / RAND_MAX;
	if (randomnum <= m_TractorBeamChance * dt)
	{
		auto nextState{std::make_unique<TractorBeam>()};

		int xOffsetFromSides{ 50 };
		float seekXValue{ float(rand() % (WINDOW_WIDTH - 2 * xOffsetFromSides) + xOffsetFromSides) };
		auto seekPos = glm::vec2{ seekXValue, 300.f };

		return std::move(std::make_unique<AE::States::Seek>(std::move(nextState), seekPos));
	}
	return nullptr;
}

//****
// BombingRun
//****
void StatesEnemyBoss::BombingRun::OnEnter(AE::GameObject* gameObject)
{
	m_SeekPos.x = rand() % 2 ? 0.f : WINDOW_WIDTH;
	m_SeekDir = glm::normalize(m_SeekPos - glm::vec2{gameObject->GetLocalTransform().GetPosition()});

}
std::unique_ptr<AE::FSMState> StatesEnemyBoss::BombingRun::Update(AE::GameObject* gameObject)
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

	if (yHeight > WINDOW_HEIGHT)
	{
		gameObject->AddLocalTransform(AE::Transform{ 0.f, -(WINDOW_HEIGHT + 100.f)});

		auto nextState{ std::make_unique<Idle>() };
		return std::move(std::make_unique<AE::States::Seek>(std::move(nextState), glm::vec2{ gameObject->GetSpawnTransform().GetPosition() }));
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
std::unique_ptr<AE::FSMState> StatesEnemyBoss::TractorBeam::Update(AE::GameObject* gameObject)
{
	m_CurrentDuration += AE::Time::GetInstance().GetDeltaTime();

	if (m_CurrentDuration >= m_BeamDuration)
	{
		if (m_BeamHitbox)
		{
			m_BeamHitbox->SetActive(true);

			auto nextState{ std::make_unique<Idle>() };
			return std::move(std::make_unique<AE::States::Seek>(std::move(nextState), glm::vec2{ gameObject->GetSpawnTransform().GetPosition() }));
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

	// Animation
	auto animComp{ std::make_shared<AnimationComponent>(m_BeamGO.get(), "TractorBeam.png", 18, 3.f / 18) };
	animComp->SetDestRect({ 0, 0, 80, 140 });
	animComp->SetSourceRect({ 0, 0, 48, 80 });
	m_BeamGO->AddComponent(animComp);

	AE::SceneManager::GetInstance().GetCurrentScene()->Add(m_BeamGO);
}

//****
// TwoHealth
//****
void StatesEnemyBoss::FullHealth::OnEnter(AE::GameObject* gameObject)
{
	if (auto imageComp = gameObject->GetComponent<ImageComponent>())
	{
		imageComp->SetSourcePos(glm::vec2{ 1.f, 91.f });
	}

	m_HealthComp = gameObject->GetComponent<HealthComponent>();
}
std::unique_ptr<AE::FSMState> StatesEnemyBoss::FullHealth::Update(AE::GameObject*)
{
	if (m_HealthComp)
	{
		if (m_HealthComp->GetHealth() <= 1)
		{
			return std::move(std::make_unique<HalfHealth>());
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
		imageComp->SetSourcePos(glm::vec2{ 1.f, 127.f });
	}
}
std::unique_ptr<AE::FSMState> StatesEnemyBoss::HalfHealth::Update(AE::GameObject* )
{
	return nullptr;
}
