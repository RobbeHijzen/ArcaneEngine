#include "EnemyManagerComponent.h"

EnemyManagercomponent::EnemyManagercomponent(AE::GameObject* pParent)
	: BaseComponent(pParent)
{
	auto spawnerComp{ std::make_shared<EnemySpawnerComponent>(pParent) };
	m_EnemySpawnerComp = spawnerComp.get();

	GetOwner()->AddComponent(spawnerComp);
}

void EnemyManagercomponent::Update()
{
}
