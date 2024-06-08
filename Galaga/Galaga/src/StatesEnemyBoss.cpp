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

	float randomHeightDif{rand() / RAND_MAX * 2 * m_RandomHeightOffset - m_RandomHeightOffset };
	m_ShootHeight += randomHeightDif;
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
				if (auto shootCompBase = gameObject->GetComponent<ShootComponent>())
				{
					shootCompBase->FireBullet();
				}
				for (const auto& child : gameObject->GetChildren())
				{
					if (auto shootComp = child->GetComponent<ShootComponent>())
					{
						shootComp->FireBullet();
					}
				}
			}, m_ShootDelay, m_ShootAmount, true);
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
std::unique_ptr<AE::FSMState> StatesEnemyBoss::TractorBeam::Update(AE::GameObject* )
{
	if (m_SwitchToBeamSuck)
	{
		return std::move(std::make_unique<StatesEnemyBoss::BeamSuck>(m_IdlePos, m_PlayerGO, m_BeamGO));
	}

	m_CurrentDuration += AE::Time::GetInstance().GetDeltaTime();

	if (m_CurrentDuration >= m_BeamDuration)
	{
		m_BeamGO->Delete();

		auto nextState{ std::make_unique<Idle>(m_IdlePos) };

		std::list<EnemySeekInfo> seekInfos{};
		seekInfos.emplace_back(EnemySeekInfo{ EnemySeekTypes::Straight, m_IdlePos });
		return std::move(std::make_unique<StatesEnemy::Moving>(std::move(nextState), seekInfos));
	}

	return nullptr;
}
void StatesEnemyBoss::TractorBeam::SpawnBeam(AE::GameObject* go)
{
	m_BeamGO = std::make_shared<AE::GameObject>();
	m_BeamGO->SetLocalTransform(go->GetWorldTransform());
	m_BeamGO->AddLocalTransform(AE::Transform{ -21.3f, 40.f });
	m_BeamGO->AddObserver(std::move(std::make_unique<BeamObserver>()));
	m_BeamGO->AddTag("Beam");

	auto beamHitbox{ std::make_shared<HitboxComponent>(m_BeamGO.get(), 80.f, 140.f) };
	beamHitbox->AddIgnoreTag("Enemy");

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
// BeamSuck
//****
std::unique_ptr<AE::FSMState> StatesEnemyBoss::BeamSuck::Update(AE::GameObject* gameObject)
{
	if (m_MovePlayer)
	{
		glm::vec2 enemyPos{ glm::vec2{ gameObject->GetWorldTransform().GetPosition() } };
		glm::vec2 dir{ AE::StateHelpers::GetDirection({enemyPos.x + 10.f, enemyPos.y}, m_PlayerGO) };
		dir = glm::normalize(dir);

		dir *= m_SuckSpeed * AE::Time::GetInstance().GetDeltaTime();

		m_PlayerGO->AddLocalTransform({ dir.x, dir.y });
	}
	else if (m_ChangeState)
	{
		auto nextState{ std::make_unique<Idle>(m_IdlePos) };

		std::list<EnemySeekInfo> seekInfos{};
		seekInfos.emplace_back(EnemySeekInfo{ EnemySeekTypes::Straight, m_IdlePos });
		return std::move(std::make_unique<StatesEnemy::Moving>(std::move(nextState), seekInfos));
	}

	return nullptr;
}
void StatesEnemyBoss::BeamSuck::SpawnRedGalaga(AE::GameObject* enemyGO)
{
	m_MovePlayer = false;
	m_BeamGO->Delete();
	AE::TimeManager::GetInstance().SetTimer([&](int){ m_ChangeState = true; }, m_AFKTimeAfterKill, 1);

	auto galaga = std::make_shared<AE::GameObject>();
	galaga->SetLocalTransform({ 0.f, -40.f });
	galaga->AddTag("Enemy");

	// Image Component
	auto imageComp{ std::make_shared<MovementImageComponent>(galaga.get(), "Galaga.png", glm::vec2{2.f, 0.f}, 16, glm::vec2{0.f, 1.f}) };
	imageComp->SetDestRect({ 0, 0, 34, 34 });
	imageComp->SetSourceRect({ 1, 181, 16, 16 });
	galaga->AddComponent(imageComp);

	// Shoot Component
	auto shootComp{ std::make_shared<ShootComponent>(galaga.get()) };
	shootComp->SetBulletDirection(glm::vec2{ 0.f, 1.f });
	shootComp->SetBulletSpeed(200.f);
	shootComp->AddSeekTarget(m_PlayerGO);
	shootComp->AddIgnoreTag("Enemy");
	shootComp->AddIgnoreTag("Bullet");
	shootComp->SetBulletSpawnOffset(glm::vec2{ 2.5f, 30.f });
	galaga->AddComponent(shootComp);


	AE::SceneManager::GetInstance().GetCurrentScene()->Add(galaga);
	galaga->SetParent(enemyGO, false);
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
	if (auto imageComp = gameObject->GetComponent<MovementImageComponent>())
	{
		imageComp->AddSourcePos({0.f, 36.f});
	}
}
std::unique_ptr<AE::FSMState> StatesEnemyBoss::HalfHealth::Update(AE::GameObject* )
{
	return nullptr;
}
