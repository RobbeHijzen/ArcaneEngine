#pragma once

#include "BaseComponent.h"
#include "GameObject.h"
#include "EnemySpawnerComponent.h"

class EnemyManagercomponent : public AE::BaseComponent
{
public:

	EnemyManagercomponent(AE::GameObject* pParent);

	virtual void Update() override;

private:

	EnemySpawnerComponent* m_EnemySpawnerComp{};

	std::vector<AE::GameObject*> m_BeeEnemies{};
	std::vector<AE::GameObject*> m_ButterflyEnemies{};
	std::vector<AE::GameObject*> m_BossEnemies{};

};


