#include "EnemySpawnerComponent.h"
#include "ArcaneEngine.h"

#include "Observers.h"

#include "ImageComponent.h"
#include "ShootComponent.h"
#include "HealthComponent.h"

#include "FSMComponent.h"
#include "StatesEnemyBoss.h"
#include "StatesEnemy.h"

#include <queue>

void EnemySpawnerComponent::SpawnBeeEnemy(glm::vec2 )
{
	std::cout << "Bee Spawned\n";
}

void EnemySpawnerComponent::SpawnButterflyEnemy(glm::vec2 )
{
	std::cout << "Butterfly Spawned\n";
}

void EnemySpawnerComponent::SpawnBossEnemy(std::queue<EnemySeekInfo> seekInfo)
{
	auto enemy = std::make_shared<AE::GameObject>();
	enemy->SetLocalTransform(GetWorldTransform());
	enemy->AddTag("Enemy");

	// Image Component
	auto imageComp{ std::make_shared<ImageComponent>(enemy.get(), "Galaga.png") };
	imageComp->SetDestRect({ 0, 0, 40, 40 });
	imageComp->SetSourceRect({ 1, 91, 16, 16 });
	enemy->AddComponent(imageComp);

	// Shoot Component
	auto shootComp{ std::make_shared<ShootComponent>(enemy.get()) };
	shootComp->SetBulletDirection(glm::vec2{ 0.f, 1.f });
	shootComp->SetBulletSpeed(200.f);
	shootComp->SetSeekTarget(m_Galaga);
	shootComp->AddIgnoreTag("Enemy");
	shootComp->SetBulletSpawnOffset(glm::vec2{ 2.5f, 30.f });

	enemy->AddComponent(shootComp);

	// Other Components
	enemy->AddComponent(std::make_shared<HealthComponent>(enemy.get(), 2));
	enemy->AddComponent(std::make_shared<HitboxComponent>(enemy.get(), 40.f, 40.f));

	// Observers
	enemy->AddObserver(std::move(std::make_unique<EnemyObserver>(m_Galaga, 150)));

	// FSM

	// Seek Infos
	auto spawningState{ std::make_unique<StatesEnemy::Spawning>(std::move(std::make_unique<StatesEnemyBoss::Idle>()), seekInfo) };
	enemy->AddComponent(std::make_shared<FSMComponent>(enemy.get(), std::move(spawningState)));

	// Texture FSM
	auto fullHealthState{ std::make_unique<StatesEnemyBoss::FullHealth>() };
	enemy->AddComponent(std::make_shared<FSMComponent>(enemy.get(), std::move(fullHealthState)));

	AE::SceneManager::GetInstance().GetCurrentScene()->Add(enemy);
}
