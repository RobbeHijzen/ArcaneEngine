#pragma once

#include "BaseComponent.h"
#include "GameObject.h"
#include "EnemySpawningStructs.h"
#include <queue>

class EnemySpawnerComponent : public AE::BaseComponent
{
public:

	EnemySpawnerComponent(AE::GameObject* pParent, AE::GameObject* galaga)
		: BaseComponent(pParent)
		, m_Galaga{galaga}
	{
	}

	void SpawnBeeEnemy(std::queue<EnemySeekInfo> seekInfo);
	void SpawnButterflyEnemy(std::queue<EnemySeekInfo> seekInfo);
	void SpawnBossEnemy(std::queue<EnemySeekInfo> seekInfo);

private:
	AE::GameObject* m_Galaga{};
};


