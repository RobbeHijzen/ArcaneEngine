#pragma once

#include "BaseComponent.h"
#include "GameObject.h"
#include "ArcaneEngine.h"

#include <queue>

class EnemySpawnerComponent;

struct WaveInfo
{
	std::queue<glm::vec2> beeEnemies{};
	std::queue<glm::vec2> butterflyEnemies{};
	std::queue<glm::vec2> bossEnemies{};
};

class SpawnerManagerComponent : public AE::BaseComponent
{
public:

	SpawnerManagerComponent(AE::GameObject* pParent, AE::GameObject* galaga);

	virtual void GameStart() override;

	void SpawnWave();

	void RemoveEnemy(AE::GameObject* toRemoveObject)
	{ 
		m_Bees.remove(toRemoveObject);
		m_Butterflies.remove(toRemoveObject);
		m_Bosses.remove(toRemoveObject);

		if (m_Bees.size() + m_Butterflies.size() + m_Bosses.size() <= 0)
		{
			SpawnWave();
		}
	}

private:

	enum class SpawningTypes
	{
		Bees,
		Butterflies,
		Bosses
	};


	std::vector<EnemySpawnerComponent*> m_EnemySpawners{};
	std::vector<WaveInfo> m_WaveInfos{};

	void CreateSpawners(AE::GameObject* pParent, AE::GameObject* galaga);

	void FillWaveInfos();
	std::string RemoveUntilChar(std::string& input, char stopChar);

	// Wave Info stuff
	void SpawnPhase(int spawningOrderIndex);
	void SpawnNextEnemy(SpawningTypes spawningType, int spawnerIndex);

	WaveInfo m_CurrentWaveInfo{};
	bool m_CurrentlySpawningWave{};
	int m_CurrentWaveInfoIndex{};

	float m_CurrentSectionSpawnTimer{0.f};
	float m_SectionSpawnInterval{0.5f};


	std::vector<std::pair<int, std::pair<SpawningTypes, SpawningTypes>>> m_SpawningOrder 
		// one spawning phase has an int(spawner index) and two types of enemies (std::pair), can be the same
	{
		{ 1, {SpawningTypes::Bees, SpawningTypes::Butterflies}},
		{ 2, {SpawningTypes::Butterflies, SpawningTypes::Bosses}},
		{ 0, {SpawningTypes::Butterflies, SpawningTypes::Butterflies}},
		{ 1, {SpawningTypes::Bees, SpawningTypes::Bees}},
		{ 2, {SpawningTypes::Bees, SpawningTypes::Bees}}
	};

	std::list<AE::GameObject*> m_Bees{};
	std::list<AE::GameObject*> m_Butterflies{};
	std::list<AE::GameObject*> m_Bosses{};


};


