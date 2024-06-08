#include "EnemySpawnerComponent.h"
#include "ArcaneEngine.h"

#include "Observers.h"

#include "ImageComponent.h"
#include "ShootComponent.h"
#include "HealthComponent.h"
#include "MovementImageComponent.h"

#include "FSMComponent.h"
#include "StatesEnemyBoss.h"
#include "StatesEnemyBee.h"
#include "StatesEnemyButterfly.h"
#include "StatesEnemy.h"

#include <queue>

AE::GameObject* EnemySpawnerComponent::SpawnBeeEnemy(std::list<EnemySeekInfo> seekInfo, SpawnerManagerComponent* spawnerManagerComp)
{
	auto enemy = std::make_shared<AE::GameObject>();
	enemy->SetLocalTransform(GetWorldTransform());
	enemy->AddTag("Enemy");

	// Image Component
	auto imageComp{ std::make_shared<MovementImageComponent>(enemy.get(), "Galaga.png", glm::vec2{2.f, 0.f}, 16, glm::vec2{0.f, 1.f}) };
	imageComp->SetDestRect({ 0, 0, 34, 34 });
	imageComp->SetSourceRect({ 1, 19 + 18, 16, 16 });
	enemy->AddComponent(imageComp);

	// Shoot Component
	auto shootComp{ std::make_shared<ShootComponent>(enemy.get()) };
	shootComp->SetBulletDirection(glm::vec2{ 0.f, 1.f });
	shootComp->SetBulletSpeed(200.f);
	for (const auto& galaga : m_Galagas)
	{
		shootComp->AddSeekTarget(galaga);
	}
	shootComp->AddIgnoreTag("Enemy");
	shootComp->AddIgnoreTag("Bullet");
	shootComp->SetBulletSpawnOffset(glm::vec2{ 2.5f, 30.f });

	enemy->AddComponent(shootComp);

	// Other Components
	enemy->AddComponent(std::make_shared<HealthComponent>(enemy.get(), 1));
	auto hitboxComp{ std::make_shared<HitboxComponent>(enemy.get(), 24.f, 24.f) };
	hitboxComp->AddLocalPosition(5.f, 5.f);
	enemy->AddComponent(hitboxComp);

	// Observers
	enemy->AddObserver(std::move(std::make_unique<EnemyObserver>(m_Galagas.front(), 50, 100)));
	enemy->AddObserver(std::move(std::make_unique<StateMachineObserver>(spawnerManagerComp)));

	// FSM Seek Infos
	auto spawningState{ std::make_unique<StatesEnemy::Moving>(std::move(std::make_unique<StatesEnemyBee::Idle>(seekInfo.back().seekPos)), seekInfo) };
	enemy->AddComponent(std::make_shared<FSMComponent>(enemy.get(), std::move(spawningState)));


	AE::SceneManager::GetInstance().GetCurrentScene()->Add(enemy);
	return enemy.get();
}

AE::GameObject* EnemySpawnerComponent::SpawnButterflyEnemy(std::list<EnemySeekInfo> seekInfo, SpawnerManagerComponent* spawnerManagerComp)
{
	auto enemy = std::make_shared<AE::GameObject>();
	enemy->SetLocalTransform(GetWorldTransform());
	enemy->AddTag("Enemy");

	// Image Component
	auto imageComp{ std::make_shared<MovementImageComponent>(enemy.get(), "Galaga.png", glm::vec2{2.f, 0.f}, 16, glm::vec2{0.f, 1.f}) };
	imageComp->SetDestRect({ 0, 0, 34, 34 });
	imageComp->SetSourceRect({ 1, 55 + 18, 16, 16 });
	enemy->AddComponent(imageComp);

	// Shoot Component
	auto shootComp{ std::make_shared<ShootComponent>(enemy.get()) };
	shootComp->SetBulletDirection(glm::vec2{ 0.f, 1.f });
	shootComp->SetBulletSpeed(200.f);
	for (const auto& galaga : m_Galagas)
	{
		shootComp->AddSeekTarget(galaga);
	}
	shootComp->AddIgnoreTag("Enemy");
	shootComp->AddIgnoreTag("Bullet");
	shootComp->SetBulletSpawnOffset(glm::vec2{ 2.5f, 30.f });

	enemy->AddComponent(shootComp);

	// Other Components
	enemy->AddComponent(std::make_shared<HealthComponent>(enemy.get(), 1));
	auto hitboxComp{ std::make_shared<HitboxComponent>(enemy.get(), 24.f, 24.f) };
	hitboxComp->AddLocalPosition(5.f, 5.f);
	enemy->AddComponent(hitboxComp);

	// Observers
	enemy->AddObserver(std::move(std::make_unique<EnemyObserver>(m_Galagas.front(), 80, 160)));
	enemy->AddObserver(std::move(std::make_unique<StateMachineObserver>(spawnerManagerComp)));

	// FSM Seek Infos
	auto spawningState{ std::make_unique<StatesEnemy::Moving>(std::move(std::make_unique<StatesEnemyButterfly::Idle>(seekInfo.back().seekPos)), seekInfo) };
	enemy->AddComponent(std::make_shared<FSMComponent>(enemy.get(), std::move(spawningState)));


	AE::SceneManager::GetInstance().GetCurrentScene()->Add(enemy);
	return enemy.get();
}

AE::GameObject* EnemySpawnerComponent::SpawnBossEnemy(std::list<EnemySeekInfo> seekInfo, SpawnerManagerComponent* spawnerManagerComp)
{
	auto enemy = std::make_shared<AE::GameObject>();
	enemy->SetLocalTransform(GetWorldTransform());
	enemy->AddTag("Enemy");

	// Image Component
	auto imageComp{ std::make_shared<MovementImageComponent>(enemy.get(), "Galaga.png", glm::vec2{2.f, 0.f}, 16, glm::vec2{0.f, 1.f}) };
	imageComp->SetDestRect({ 0, 0, 34, 34 });
	imageComp->SetSourceRect({ 1, 109, 16, 16 });
	enemy->AddComponent(imageComp);

	// Shoot Component
	auto shootComp{ std::make_shared<ShootComponent>(enemy.get()) };
	shootComp->SetBulletDirection(glm::vec2{ 0.f, 1.f });
	shootComp->SetBulletSpeed(200.f);
	for (const auto& galaga : m_Galagas)
	{
		shootComp->AddSeekTarget(galaga);
	}
	shootComp->AddIgnoreTag("Enemy");
	shootComp->AddIgnoreTag("Bullet");
	shootComp->SetBulletSpawnOffset(glm::vec2{ 2.5f, 30.f });

	enemy->AddComponent(shootComp);

	// Other Components
	enemy->AddComponent(std::make_shared<HealthComponent>(enemy.get(), 2));
	auto hitboxComp{ std::make_shared<HitboxComponent>(enemy.get(), 29.f, 29.f) };
	hitboxComp->AddLocalPosition(2.5f, 2.5f);
	enemy->AddComponent(hitboxComp);

	// Observers
	enemy->AddObserver(std::move(std::make_unique<EnemyObserver>(m_Galagas.front(), 150, 400)));
	enemy->AddObserver(std::move(std::make_unique<StateMachineObserver>(spawnerManagerComp)));
	enemy->AddObserver(std::move(std::make_unique<EnemyBossObserver>()));

	// FSM

	// Seek Infos
	auto spawningState{ std::make_unique<StatesEnemy::Moving>(std::move(std::make_unique<StatesEnemyBoss::Idle>(seekInfo.back().seekPos)), seekInfo) };
	enemy->AddComponent(std::make_shared<FSMComponent>(enemy.get(), std::move(spawningState)));

	// Texture FSM
	auto fullHealthState{ std::make_unique<StatesEnemyBoss::FullHealth>(glm::vec2{1, 109}) };
	enemy->AddComponent(std::make_shared<FSMComponent>(enemy.get(), std::move(fullHealthState)));

	AE::SceneManager::GetInstance().GetCurrentScene()->Add(enemy);
	return enemy.get();
}
