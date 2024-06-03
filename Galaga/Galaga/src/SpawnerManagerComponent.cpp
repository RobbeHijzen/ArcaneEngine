#include "SpawnerManagerComponent.h"
#include "MyTime.h"
#include "EnemySpawningStructs.h"
#include "TimeManager.h"

#include <fstream>
#include <algorithm>

SpawnerManagercomponent::SpawnerManagercomponent(AE::GameObject* pParent, AE::GameObject* galaga)
	: BaseComponent(pParent)
{
	CreateSpawners(pParent, galaga);
	FillWaveInfos();

	m_WaveInfos[0].butterflyEnemies.push({ 150.f, 100.f });
	m_WaveInfos[0].butterflyEnemies.push({ 180.f, 100.f });
	m_WaveInfos[0].butterflyEnemies.push({ 210.f, 100.f });
	m_WaveInfos[0].butterflyEnemies.push({ 240.f, 100.f });
	m_WaveInfos[0].butterflyEnemies.push({ 270.f, 100.f });
	m_WaveInfos[0].butterflyEnemies.push({ 300.f, 100.f });
	m_WaveInfos[0].butterflyEnemies.push({ 330.f, 100.f });
	m_WaveInfos[0].butterflyEnemies.push({ 360.f, 100.f });

	m_WaveInfos[0].beeEnemies.push({ 150.f, 150.f });
	m_WaveInfos[0].beeEnemies.push({ 180.f, 150.f });
	m_WaveInfos[0].beeEnemies.push({ 210.f, 150.f });
	m_WaveInfos[0].beeEnemies.push({ 240.f, 150.f });
	m_WaveInfos[0].beeEnemies.push({ 270.f, 150.f });
	m_WaveInfos[0].beeEnemies.push({ 300.f, 150.f });
	m_WaveInfos[0].beeEnemies.push({ 330.f, 150.f });
	m_WaveInfos[0].beeEnemies.push({ 360.f, 150.f });
}

void SpawnerManagercomponent::GameStart()
{
	SpawnWave();
}

void SpawnerManagercomponent::SpawnWave()
{
	if (!m_CurrentlySpawningWave)
	{
		m_CurrentWaveInfo = m_WaveInfos[m_CurrentWaveInfoIndex++];
		m_CurrentlySpawningWave = true;
		m_PhaseSpawnTimerIndex = AE::TimeManager::GetInstance().SetTimer([&](int itNum) 
			{
				SpawnPhase(itNum % int(m_SpawningOrder.size()));

			}, 10.f, 10,
			[&]() {m_CurrentlySpawningWave = false; });

	}
}
void SpawnerManagercomponent::SpawnPhase(int spawningOrderIndex)
{
	AE::TimeManager::GetInstance().SetTimer([&, spawningOrderIndex](int itNum)
		{
			if (m_PhaseTimerHasBeenCleared) return;
			if (m_CurrentWaveInfo.beeEnemies.empty() && m_CurrentWaveInfo.butterflyEnemies.empty() && m_CurrentWaveInfo.bossEnemies.empty())
			{
				m_PhaseTimerHasBeenCleared = true;
				AE::TimeManager::GetInstance().ClearTimer(m_PhaseSpawnTimerIndex);
				return;
			}

			if (itNum % 2)
			{
				SpawnNextEnemy(m_SpawningOrder[spawningOrderIndex].second.first, m_SpawningOrder[spawningOrderIndex].first);
			}
			else
			{
				SpawnNextEnemy(m_SpawningOrder[spawningOrderIndex].second.second, m_SpawningOrder[spawningOrderIndex].first);
			}
		}, 0.4f, 8);
}
void SpawnerManagercomponent::SpawnNextEnemy(SpawningTypes spawningType, int spawnerIndex)
{
	switch (spawningType)
	{
	case SpawningTypes::Bees:
	{
		if (m_CurrentWaveInfo.beeEnemies.size() > 0)
		{
			m_EnemySpawners[spawnerIndex]->SpawnBeeEnemy(m_CurrentWaveInfo.beeEnemies.front());
			m_CurrentWaveInfo.beeEnemies.pop();
		}
		break;
	}
	case SpawningTypes::Butterflies:
	{
		if (m_CurrentWaveInfo.butterflyEnemies.size() > 0)
		{
			m_EnemySpawners[spawnerIndex]->SpawnButterflyEnemy(m_CurrentWaveInfo.butterflyEnemies.front());
			m_CurrentWaveInfo.butterflyEnemies.pop();
		}
		break;
	}
	case SpawningTypes::Bosses:
	{
		if (m_CurrentWaveInfo.bossEnemies.size() > 0)
		{
			std::queue<EnemySeekInfo> seekInfo{};
			glm::vec2 firstSeekPos{ m_CurrentWaveInfo.bossEnemies.size() % 2 ? m_MiddlePointLeft : m_MiddlePointRight };
			seekInfo.push(EnemySeekInfo{ EnemySeekTypes::Straight, firstSeekPos });
			seekInfo.push(EnemySeekInfo{ EnemySeekTypes::Straight, m_CurrentWaveInfo.bossEnemies.front() });

			m_EnemySpawners[spawnerIndex]->SpawnBossEnemy(seekInfo);
			m_CurrentWaveInfo.bossEnemies.pop();
		}
		break;
	}
	default: break;
	}
}

void SpawnerManagercomponent::CreateSpawners(AE::GameObject* pParent, AE::GameObject* galaga)
{
	auto spawnerComp_01{ std::make_shared<EnemySpawnerComponent>(pParent, galaga) };
	spawnerComp_01->AddLocalPosition(320.f, -30.f);
	m_EnemySpawners.emplace_back(spawnerComp_01.get());

	auto spawnerComp_02{ std::make_shared<EnemySpawnerComponent>(pParent, galaga) };
	spawnerComp_02->AddLocalPosition(-30.f, 480.f);
	m_EnemySpawners.emplace_back(spawnerComp_02.get());

	auto spawnerComp_03{ std::make_shared<EnemySpawnerComponent>(pParent, galaga) };
	spawnerComp_03->AddLocalPosition(670.f, 480.f);
	m_EnemySpawners.emplace_back(spawnerComp_03.get());

	GetOwner()->AddComponent(spawnerComp_01);
	GetOwner()->AddComponent(spawnerComp_02);
	GetOwner()->AddComponent(spawnerComp_03);
}

void SpawnerManagercomponent::FillWaveInfos()
{
	if (std::ifstream input{ "../Resources/Formations/Wave1.txt" }; input.is_open())
	{
		WaveInfo waveInfo{};

		while (!input.eof())
		{
			

			std::string line{};
			if (!std::getline(input, line)) break;

			try
			{
				std::string enemyIndexString{ RemoveUntilChar(line, ' ')};
				int enemyIndex{std::stoi(enemyIndexString)};
				
				RemoveUntilChar(line, '[');
				std::string xValue{ RemoveUntilChar(line, ',') };
				RemoveUntilChar(line, ' ');
				std::string yValue{ RemoveUntilChar(line, ']') };
				glm::vec2 pos{ std::stof(xValue), std::stof(yValue) };

				switch (enemyIndex)
				{
				case 0: waveInfo.beeEnemies.push(pos); break;
				case 1: waveInfo.butterflyEnemies.push(pos); break;
				case 2: waveInfo.bossEnemies.push(pos); break;
				}

			}
			catch (...)
			{
				continue;
			}
		}
		m_WaveInfos.emplace_back(waveInfo);

	}
}
std::string SpawnerManagercomponent::RemoveUntilChar(std::string& input, char stopChar)
{
	std::string result{};
	for (std::size_t i = 0; i < input.size(); ++i) 
	{
		if (input.front() == stopChar)
		{
			input.erase(input.begin());
			break;
		}
		result += input.front();
		input.erase(input.begin());
	}
	return result;
}
