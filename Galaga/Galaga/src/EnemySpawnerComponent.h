#pragma once

#include "BaseComponent.h"
#include "GameObject.h"

class EnemySpawnerComponent : public AE::BaseComponent
{
public:

	EnemySpawnerComponent(AE::GameObject* pParent)
		: BaseComponent(pParent)
	{
	}

	void SpawnBeeEnemy();
	void SpawnButterflyEnemy();
	void SpawnBossEnemy();

private:

};


