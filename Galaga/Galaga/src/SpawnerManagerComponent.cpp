#include "SpawnerManagerComponent.h"
#include "MyTime.h"
#include "EnemySpawningStructs.h"
#include "TimeManager.h"
#include "EnemySpawnerComponent.h"
#include "Observers.h"
#include "FSMComponent.h"
#include "StatesEnemyBoss.h"

#include <fstream>
#include <algorithm>

SpawnerManagerComponent::SpawnerManagerComponent(AE::GameObject* pParent, AE::GameObject* galaga)
	: BaseComponent(pParent)
{
	CreateSpawners(pParent, galaga);
	FillWaveInfos();
}

void SpawnerManagerComponent::GameStart()
{
	SpawnWave();
}

void SpawnerManagerComponent::SpawnWave()
{
	if (!m_CurrentlySpawningWave)
	{
		m_CurrentWaveInfo = m_WaveInfos[m_CurrentWaveInfoIndex];
		m_CurrentWaveInfoIndex = m_CurrentWaveInfoIndex % m_WaveInfos.size();

		m_CurrentlySpawningWave = true;
		AE::TimeManager::GetInstance().SetTimer([&](int itNum) 
			{
				SpawnPhase(itNum % int(m_SpawningOrder.size()));

			}, 10.f, 5, true,
			[&]() 
				{
					m_CurrentlySpawningWave = false; 
					AE::TimeManager::GetInstance().SetTimer([&](int )
						{
							if (auto fsmComp = m_Bosses.front()->GetComponent<FSMComponent>())
							{
								if (auto idle = dynamic_cast<StatesEnemyBoss::Idle*>(fsmComp->GetCurrentState()))
								{
									idle->ChangeToTractorBeam();
								}
							}
						}, 10.f, 1);
				});

	}
}
void SpawnerManagerComponent::SpawnPhase(int spawningOrderIndex)
{
	AE::TimeManager::GetInstance().SetTimer([&, spawningOrderIndex](int itNum)
		{
			if (m_CurrentWaveInfo.beeEnemies.empty() && m_CurrentWaveInfo.butterflyEnemies.empty() && m_CurrentWaveInfo.bossEnemies.empty())
			{
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
void SpawnerManagerComponent::SpawnNextEnemy(SpawningTypes spawningType, int spawnerIndex)
{
	switch (spawningType)
	{
	case SpawningTypes::Bees:
	{
		if (m_CurrentWaveInfo.beeEnemies.size() > 0)
		{
			std::list<EnemySeekInfo> seekInfo{};
			glm::vec2 firstSeekPos{ m_EnemySpawners[spawnerIndex]->GetFirstSeekPoint()};
			seekInfo.emplace_back(EnemySeekInfo{ EnemySeekTypes::Circle, firstSeekPos });
			seekInfo.emplace_back(EnemySeekInfo{ EnemySeekTypes::Straight, m_CurrentWaveInfo.beeEnemies.front() });

			m_Bees.emplace_back(m_EnemySpawners[spawnerIndex]->SpawnBeeEnemy(seekInfo));
			m_Bees.back()->AddObserver(std::move(std::make_unique<SpawnedObjectObserver>(this)));

			m_CurrentWaveInfo.beeEnemies.pop();
		}
		break;
	}
	case SpawningTypes::Butterflies:
	{
		if (m_CurrentWaveInfo.butterflyEnemies.size() > 0)
		{
			std::list<EnemySeekInfo> seekInfo{};
			glm::vec2 firstSeekPos{ m_EnemySpawners[spawnerIndex]->GetFirstSeekPoint() };
			seekInfo.emplace_back(EnemySeekInfo{ EnemySeekTypes::Circle, firstSeekPos });
			seekInfo.emplace_back(EnemySeekInfo{ EnemySeekTypes::Straight, m_CurrentWaveInfo.butterflyEnemies.front() });

			m_Butterflies.emplace_back(m_EnemySpawners[spawnerIndex]->SpawnButterflyEnemy(seekInfo));
			m_Butterflies.back()->AddObserver(std::move(std::make_unique<SpawnedObjectObserver>(this)));

			m_CurrentWaveInfo.butterflyEnemies.pop();
		}
		break;
	}
	case SpawningTypes::Bosses:
	{
		if (m_CurrentWaveInfo.bossEnemies.size() > 0)
		{
			std::list<EnemySeekInfo> seekInfo{};
			glm::vec2 firstSeekPos{ m_EnemySpawners[spawnerIndex]->GetFirstSeekPoint() };
			seekInfo.emplace_back(EnemySeekInfo{ EnemySeekTypes::Circle, firstSeekPos });
			seekInfo.emplace_back(EnemySeekInfo{ EnemySeekTypes::Straight, m_CurrentWaveInfo.bossEnemies.front() });

			m_Bosses.emplace_back(m_EnemySpawners[spawnerIndex]->SpawnBossEnemy(seekInfo));
			m_Bosses.back()->AddObserver(std::move(std::make_unique<SpawnedObjectObserver>(this)));

			m_CurrentWaveInfo.bossEnemies.pop();
		}
		break;
	}
	default: break;
	}
}

void SpawnerManagerComponent::CreateSpawners(AE::GameObject* pParent, AE::GameObject* galaga)
{
	auto spawnerComp_01{ std::make_shared<EnemySpawnerComponent>(pParent, galaga, glm::vec2{300.f, 220.f}) };
	spawnerComp_01->AddLocalPosition(320.f, -30.f);
	m_EnemySpawners.emplace_back(spawnerComp_01.get());

	auto spawnerComp_02{ std::make_shared<EnemySpawnerComponent>(pParent, galaga, glm::vec2{150.f, 300.f}) };
	spawnerComp_02->AddLocalPosition(-30.f, 480.f);
	m_EnemySpawners.emplace_back(spawnerComp_02.get());

	auto spawnerComp_03{ std::make_shared<EnemySpawnerComponent>(pParent, galaga, glm::vec2{WINDOW_WIDTH - 150.f, 300.f}) };
	spawnerComp_03->AddLocalPosition(670.f, 480.f);
	m_EnemySpawners.emplace_back(spawnerComp_03.get());

	GetOwner()->AddComponent(spawnerComp_01);
	GetOwner()->AddComponent(spawnerComp_02);
	GetOwner()->AddComponent(spawnerComp_03);
}

void SpawnerManagerComponent::FillWaveInfos()
{
	if (std::ifstream input{ "../Resources/Formations/Wave1.txt" }; input.is_open())
	{
		WaveInfo waveInfo{};

		while (!input.eof())
		{
			

			std::string line{};
			if (!std::getline(input, line)) break;
			if (line.empty()) continue;

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
std::string SpawnerManagerComponent::RemoveUntilChar(std::string& input, char stopChar)
{
	std::string result{};
	while(!input.empty())
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

