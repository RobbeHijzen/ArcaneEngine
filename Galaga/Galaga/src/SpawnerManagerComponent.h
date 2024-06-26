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

	SpawnerManagerComponent(AE::GameObject* pParent, std::vector<AE::GameObject*> galagas);

	virtual void GameStart() override;

	void SpawnWave();

	void RemoveEnemy(AE::GameObject* toRemoveObject)
	{ 
		m_Bees.remove_if([&](const std::pair<AE::GameObject*, bool>& obj) { return obj.first == toRemoveObject; });
		m_Butterflies.remove_if([&](const std::pair<AE::GameObject*, bool>& obj) { return obj.first == toRemoveObject; });
		if (m_Bosses.remove_if([&](const std::pair<AE::GameObject*, bool>& obj) { return obj.first == toRemoveObject; }))
		{
			if(m_Bosses.size() == 0)
				GetOwner()->NotifyAll(AE::Event::AllBossGalagasDied);
		}


		if (m_Bees.size() + m_Butterflies.size() + m_Bosses.size() <= 0)
		{
			SpawnWave();
		}
		else
		{
			CheckForAllIdleEnemies();
		}
	}
	void SkipToNextWave();

	void ChangeStateToIdle(AE::GameObject* toChangeObject);
	void ChangeStateToNotIdle(AE::GameObject* toChangeObject);

	void DeleteAllBullets();

	AE::GameObject* GetFirstBossGalaga() 
	{ 
		if (m_Bosses.size() == 0) return nullptr;
		return m_Bosses.front().first; 
	}
	AE::GameObject* GetRandomAliveBossGalaga()
	{
		if (m_Bosses.size() == 0) return nullptr;

		int randomIndex{rand() % int(m_Bosses.size())};
		auto it{ m_Bosses.begin() };
		int idx{};
		while (it != m_Bosses.end())
		{
			if (idx++ == randomIndex) return (*it).first;
			++it;
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

	void CreateSpawners(AE::GameObject* pParent, std::vector<AE::GameObject*> galagas);

	void FillWaveInfos(const std::string& file);
	std::string RemoveUntilChar(std::string& input, char stopChar);

	// Wave Info stuff
	void SpawnPhase(int spawningOrderIndex);
	void SpawnNextEnemy(SpawningTypes spawningType, int spawnerIndex);

	WaveInfo m_CurrentWaveInfo{};
	int m_WaveTimerHandle{};
	int m_PhaseTimerHandle{};
	int m_CurrentWaveInfoIndex{};

	// Spawning Info
	float m_PhaseSpawnTime{ 6.f };
	int m_PhaseCount{5};

	float m_EnemySpawnTimeDifference{ 0.22f };
	int m_EnemySpawnCountPerPhase{8};

	std::vector<std::pair<int, std::pair<SpawningTypes, SpawningTypes>>> m_SpawningOrder 
		// one spawning phase has an int(spawner index) and two types of enemies (std::pair), can be the same
	{
		{ 1, {SpawningTypes::Bees, SpawningTypes::Butterflies}},
		{ 2, {SpawningTypes::Butterflies, SpawningTypes::Bosses}},
		{ 0, {SpawningTypes::Butterflies, SpawningTypes::Butterflies}},
		{ 1, {SpawningTypes::Bees, SpawningTypes::Bees}},
		{ 2, {SpawningTypes::Bees, SpawningTypes::Bees}}
	};

	std::list<std::pair<AE::GameObject*, bool>> m_Bees{};
	std::list<std::pair<AE::GameObject*, bool>> m_Butterflies{};
	std::list<std::pair<AE::GameObject*, bool>> m_Bosses{};


	// enemy AI handling
	void CheckForAllIdleEnemies();
	void SendAIWave();
	void SetupAIMoveSets();
	void SendEnemyOnIndexOnBombingRun(const std::list<std::pair<AE::GameObject*, bool>>& enemies, int index);
	void SendEnemyOnIndexOnTractorBeam(const std::list<std::pair<AE::GameObject*, bool>>& enemies, int index);

	std::vector<std::function<bool()>> m_EnemyAIMoveSets{};

	int m_TimesLevelSkipped{};
	int m_MaxLevelSkipTimes{3};
};


