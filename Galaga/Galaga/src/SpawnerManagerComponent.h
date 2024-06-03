#pragma once

#include "BaseComponent.h"
#include "GameObject.h"
#include "EnemySpawnerComponent.h"
#include "ArcaneEngine.h"

#include <queue>

struct WaveInfo
{
	std::queue<glm::vec2> beeEnemies{};
	std::queue<glm::vec2> butterflyEnemies{};
	std::queue<glm::vec2> bossEnemies{};
};

class SpawnerManagercomponent : public AE::BaseComponent
{
public:

	SpawnerManagercomponent(AE::GameObject* pParent, AE::GameObject* galaga);

	virtual void GameStart() override;

	void SpawnWave();

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

	int m_PhaseSpawnTimerIndex{};
	bool m_PhaseTimerHasBeenCleared{ false };

	// Seek Positions

	// Middle Points
	glm::vec2 m_MiddlePointLeft{150.f, 300.f};
	glm::vec2 m_MiddlePointRight{WINDOW_WIDTH - 150.f, 300.f};
	glm::vec2 m_MiddlePoint{ 300.f, 220.f};

	
	std::vector<std::pair<int, std::pair<SpawningTypes, SpawningTypes>>> m_SpawningOrder 
		// one spawning phase has an int(spawner index) and two types of enemies (std::pair), can be the same
	{
		{ 0, {SpawningTypes::Bees, SpawningTypes::Butterflies}},
		{ 1, {SpawningTypes::Butterflies, SpawningTypes::Bosses}},
		{ 2, {SpawningTypes::Butterflies, SpawningTypes::Butterflies}},
		{ 0, {SpawningTypes::Bees, SpawningTypes::Bees}},
		{ 1, {SpawningTypes::Bees, SpawningTypes::Bees}}
	};


};


