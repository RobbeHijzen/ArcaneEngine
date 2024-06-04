#include "StatesEnemyBoss.h"

#include "StateHelpers.h"
#include "MyTime.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Observers.h"
#include "ShootComponent.h"
#include "StatesEnemy.h"
#include "TimeManager.h"

//****
// Idle
//****
std::unique_ptr<AE::FSMState> StatesEnemyBoss::Idle::Update(AE::GameObject* gameObject)
{
	if (m_ChangeToBombingRun)
	{
		auto nextState{ std::make_unique<BombingRun>(m_StartingPos) };

		std::list<EnemySeekInfo> seekInfos{};
		seekInfos.emplace_back(EnemySeekInfo{ EnemySeekTypes::Circle, gameObject->GetWorldTransform().GetPosition() });

		return std::move(std::make_unique<StatesEnemy::Moving>(std::move(nextState), seekInfos));
	}
	else if (m_ChangeToTractorBeam)
	{
		auto nextState{std::make_unique<TractorBeam>(m_StartingPos)};

		int xOffsetFromSides{ 50 };
		float seekXValue{ float(rand() % (WINDOW_WIDTH - 2 * xOffsetFromSides) + xOffsetFromSides) };
		auto seekPos = glm::vec2{ seekXValue, 300.f };

		std::list<EnemySeekInfo> seekInfos{};
		seekInfos.emplace_back(EnemySeekInfo{ EnemySeekTypes::Circle, gameObject->GetWorldTransform().GetPosition()});
		seekInfos.emplace_back(EnemySeekInfo{ EnemySeekTypes::Straight, seekPos });

		return std::move(std::make_unique<StatesEnemy::Moving>(std::move(nextState), seekInfos));
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

		AE::TimeManager::GetInstance().SetTimer([gameObject](int )
			{
				if (auto shootComp = gameObject->GetComponent<ShootComponent>())
				{
					shootComp->FireBullet();
				}
			}, 0.5f, 2, true);
	}

	if (yHeight > WINDOW_HEIGHT)
	{
		gameObject->AddLocalTransform(AE::Transform{ 0.f, -(WINDOW_HEIGHT + 100.f)});

		auto nextState{ std::make_unique<Idle>(m_IdlePos) };

		std::list<EnemySeekInfo> seekInfos{};
		seekInfos.emplace_back(EnemySeekInfo{ EnemySeekTypes::Straight, m_IdlePos});
		return std::move(std::make_unique<StatesEnemy::Moving>(std::move(nextState), seekInfos));
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
std::unique_ptr<AE::FSMState> StatesEnemyBoss::TractorBeam::Update(AE::GameObject* )
{
	m_CurrentDuration += AE::Time::GetInstance().GetDeltaTime();

	if (m_CurrentDuration >= m_BeamDuration)
	{
		if (m_BeamHitbox)
		{
			m_BeamHitbox->SetActive(true);

			auto nextState{ std::make_unique<Idle>(m_IdlePos) };

			std::list<EnemySeekInfo> seekInfos{};
			seekInfos.emplace_back(EnemySeekInfo{ EnemySeekTypes::Straight, m_IdlePos });
			return std::move(std::make_unique<StatesEnemy::Moving>(std::move(nextState), seekInfos));
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
	m_BeamGO->SetParent(go);
}

//****
// TwoHealth
//****
void StatesEnemyBoss::FullHealth::OnEnter(AE::GameObject* gameObject)
{
	if (auto imageComp = gameObject->GetComponent<MovementImageComponent>())
	{
		imageComp->SetSourcePos(m_SourcePos);
	}

	m_HealthComp = gameObject->GetComponent<HealthComponent>();
}
std::unique_ptr<AE::FSMState> StatesEnemyBoss::FullHealth::Update(AE::GameObject*)
{
	if (m_HealthComp)
	{
		if (m_HealthComp->GetHealth() <= 1)
		{
			return std::move(std::make_unique<HalfHealth>(glm::vec2{ m_SourcePos.x, m_SourcePos.y + 36.f}));
		}
	}
	return nullptr;
}

//****
// OneHealth
//****
void StatesEnemyBoss::HalfHealth::OnEnter(AE::GameObject* gameObject)
{
	if (auto imageComp = gameObject->GetComponent<MovementImageComponent>())
	{
		imageComp->SetSourcePos(m_SourcePos);
	}
}
std::unique_ptr<AE::FSMState> StatesEnemyBoss::HalfHealth::Update(AE::GameObject* )
{
	return nullptr;
}
