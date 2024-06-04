#pragma once

#include "BaseComponent.h"
#include "GameObject.h"
#include "EnemySpawningStructs.h"
#include "SpawnerManagerComponent.h"
#include <list>

class EnemySpawnerComponent : public AE::BaseComponent
{
public:

	EnemySpawnerComponent(AE::GameObject* pParent, AE::GameObject* galaga, glm::vec2 firstSeekPoint)
		: BaseComponent(pParent)
		, m_Galaga{galaga}
		, m_FirstSeekPoint{firstSeekPoint}
	{
	}

	AE::GameObject* SpawnBeeEnemy(std::list<EnemySeekInfo> seekInfo);
	AE::GameObject* SpawnButterflyEnemy(std::list<EnemySeekInfo> seekInfo);
	AE::GameObject* SpawnBossEnemy(std::list<EnemySeekInfo> seekInfo);

	glm::vec2 GetFirstSeekPoint() const { return m_FirstSeekPoint; }

private:
	AE::GameObject* m_Galaga{};
	glm::vec2 m_FirstSeekPoint{};
};


