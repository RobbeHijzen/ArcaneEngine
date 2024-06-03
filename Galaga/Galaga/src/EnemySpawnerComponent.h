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

	void SpawnBeeEnemy(glm::vec2 endPos);
	void SpawnButterflyEnemy(glm::vec2 endPos);
	void SpawnBossEnemy(std::queue<EnemySeekInfo> seekInfo);

private:
	AE::GameObject* m_Galaga{};
};


