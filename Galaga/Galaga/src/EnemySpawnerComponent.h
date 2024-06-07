#pragma once

#include "BaseComponent.h"
#include "GameObject.h"
#include "EnemySpawningStructs.h"
#include "SpawnerManagerComponent.h"
#include <list>

class EnemySpawnerComponent : public AE::BaseComponent
{
public:

	EnemySpawnerComponent(AE::GameObject* pParent, std::vector<AE::GameObject*> galagas, glm::vec2 firstSeekPoint)
		: BaseComponent(pParent)
		, m_Galagas{galagas}
		, m_FirstSeekPoint{firstSeekPoint}
	{
	}

	AE::GameObject* SpawnBeeEnemy(std::list<EnemySeekInfo> seekInfo, SpawnerManagerComponent* spawnerManagerComp);
	AE::GameObject* SpawnButterflyEnemy(std::list<EnemySeekInfo> seekInfo, SpawnerManagerComponent* spawnerManagerComp);
	AE::GameObject* SpawnBossEnemy(std::list<EnemySeekInfo> seekInfo, SpawnerManagerComponent* spawnerManagerComp);

	glm::vec2 GetFirstSeekPoint() const { return m_FirstSeekPoint; }

private:
	std::vector<AE::GameObject*> m_Galagas{};
	glm::vec2 m_FirstSeekPoint{};
};


