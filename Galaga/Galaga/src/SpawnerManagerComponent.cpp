#include "SpawnerManagerComponent.h"
#include "MyTime.h"
#include "EnemySpawningStructs.h"
#include "TimeManager.h"
#include "EnemySpawnerComponent.h"
#include "Observers.h"
#include "FSMComponent.h"
#include "StatesEnemyBoss.h"
#include "StatesEnemyBee.h"
#include "StatesEnemyButterfly.h"

#include <fstream>
#include <algorithm>

SpawnerManagerComponent::SpawnerManagerComponent(AE::GameObject* pParent, std::vector<AE::GameObject*> galagas)
	: BaseComponent(pParent)
{
	CreateSpawners(pParent, galagas);
	FillWaveInfos("Formations/Wave1.txt");
	FillWaveInfos("Formations/Wave2.txt");
	FillWaveInfos("Formations/Wave3.txt");
	SetupAIMoveSets();

}

void SpawnerManagerComponent::GameStart()
{
	SpawnWave();
}

void SpawnerManagerComponent::SpawnWave()
{
	if (!AE::TimeManager::GetInstance().DoesTimerExist(m_WaveTimerHandle))
	{
		GetOwner()->NotifyAll(AE::Event::WaveStarted);

		m_CurrentWaveInfo = m_WaveInfos[m_CurrentWaveInfoIndex];
		m_CurrentWaveInfoIndex = (m_CurrentWaveInfoIndex + 1) % m_WaveInfos.size();

		m_WaveTimerHandle = AE::TimeManager::GetInstance().SetTimer([&](int itNum)
			{
				if (itNum == m_PhaseCount) return;
				SpawnPhase(itNum % int(m_SpawningOrder.size()));

			}, m_PhaseSpawnTime, m_PhaseCount + 1, true);

	}
}

void SpawnerManagerComponent::SkipToNextWave()
{
	++m_TimesLevelSkipped;
	if (m_TimesLevelSkipped >= m_MaxLevelSkipTimes)
	{
		AE::SceneManager::GetInstance().SetScene("DeathScreen");
		return;
	}

	AE::TimeManager::GetInstance().ClearTimer(m_WaveTimerHandle);
	AE::TimeManager::GetInstance().ClearTimer(m_PhaseTimerHandle);

	for (auto& bee : m_Bees)
		bee.first->Delete();
	for (auto& butterfly : m_Butterflies)
		butterfly.first->Delete();
	for (auto& boss : m_Bosses)
		boss.first->Delete();

	m_Bees.clear();
	m_Butterflies.clear();
	m_Bosses.clear();

	SpawnWave();
}

void SpawnerManagerComponent::ChangeStateToIdle(AE::GameObject* toChangeObject)
{
	for (auto& bee : m_Bees)
	{
		if (bee.first == toChangeObject)
		{
			bee.second = true;
			CheckForAllIdleEnemies();
			return;
		}
	}
	for (auto& butterfly : m_Butterflies)
	{
		if (butterfly.first == toChangeObject)
		{
			butterfly.second = true;
			CheckForAllIdleEnemies();
			return;
		}
	}
	for (auto& boss : m_Bosses)
	{
		if (boss.first == toChangeObject)
		{
			boss.second = true;
			CheckForAllIdleEnemies();
			return;
		}
	}
}
void SpawnerManagerComponent::ChangeStateToNotIdle(AE::GameObject* toChangeObject)
{
	for (auto& bee : m_Bees)
	{
		if (bee.first == toChangeObject)
		{
			bee.second = false;
			return;
		}
	}
	for (auto& butterfly : m_Butterflies)
	{
		if (butterfly.first == toChangeObject)
		{
			butterfly.second = false;
			return;
		}
	}
	for (auto& boss : m_Bosses)
	{
		if (boss.first == toChangeObject)
		{
			boss.second = false;
			return;
		}
	}
}

void SpawnerManagerComponent::DeleteAllBullets()
{
	for (auto& bee : m_Bees)
	{
		if (auto shootComp = bee.first->GetComponent<ShootComponent>())
		{
			shootComp->DeleteAllBullets();
		}
	}
	for (auto& butterfly : m_Butterflies)
	{
		if (auto shootComp = butterfly.first->GetComponent<ShootComponent>())
		{
			shootComp->DeleteAllBullets();
		}
	}
	for (auto& boss : m_Bosses)
	{
		if (auto shootComp = boss.first->GetComponent<ShootComponent>())
		{
			shootComp->DeleteAllBullets();
		}
	}
}

void SpawnerManagerComponent::SpawnPhase(int spawningOrderIndex)
{
	m_PhaseTimerHandle = AE::TimeManager::GetInstance().SetTimer([&, spawningOrderIndex](int itNum)
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
		}, m_EnemySpawnTimeDifference, m_EnemySpawnCountPerPhase);
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

			m_Bees.emplace_back(m_EnemySpawners[spawnerIndex]->SpawnBeeEnemy(seekInfo, this), false);
			m_Bees.back().first->AddObserver(std::move(std::make_unique<SpawnedEnemyObserver>(this)));

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

			m_Butterflies.emplace_back(m_EnemySpawners[spawnerIndex]->SpawnButterflyEnemy(seekInfo, this), false);
			m_Butterflies.back().first->AddObserver(std::move(std::make_unique<SpawnedEnemyObserver>(this)));

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

			m_Bosses.emplace_back(m_EnemySpawners[spawnerIndex]->SpawnBossEnemy(seekInfo, this), false);
			m_Bosses.back().first->AddObserver(std::move(std::make_unique<SpawnedEnemyObserver>(this)));

			m_CurrentWaveInfo.bossEnemies.pop();
		}
		break;
	}
	default: break;
	}
}

void SpawnerManagerComponent::CreateSpawners(AE::GameObject* pParent, std::vector<AE::GameObject*> galagas)
{
	auto spawnerComp_01{ std::make_shared<EnemySpawnerComponent>(pParent, galagas, glm::vec2{300.f, 220.f}) };
	spawnerComp_01->AddLocalPosition(WINDOW_WIDTH / 2.f, -30.f);
	m_EnemySpawners.emplace_back(spawnerComp_01.get());

	auto spawnerComp_02{ std::make_shared<EnemySpawnerComponent>(pParent, galagas, glm::vec2{150.f, 250.f}) };
	spawnerComp_02->AddLocalPosition(-30.f, WINDOW_HEIGHT);
	m_EnemySpawners.emplace_back(spawnerComp_02.get());

	auto spawnerComp_03{ std::make_shared<EnemySpawnerComponent>(pParent, galagas, glm::vec2{WINDOW_WIDTH - 150.f, 250.f}) };
	spawnerComp_03->AddLocalPosition(WINDOW_WIDTH + 30.f, WINDOW_HEIGHT);
	m_EnemySpawners.emplace_back(spawnerComp_03.get());

	GetOwner()->AddComponent(spawnerComp_01);
	GetOwner()->AddComponent(spawnerComp_02);
	GetOwner()->AddComponent(spawnerComp_03);
}

void SpawnerManagerComponent::FillWaveInfos(const std::string& file)
{
	if (std::ifstream input{ AE::ResourceManager::GetInstance().GetFileFromPath(file)}; input.is_open())
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

void SpawnerManagerComponent::CheckForAllIdleEnemies()
{
	if (AE::TimeManager::GetInstance().DoesTimerExist(m_WaveTimerHandle)) return;

	for (auto& bee : m_Bees)
	{
		if (!bee.second)
		{
			return;
		}
	}
	for (auto& butterfly : m_Butterflies)
	{
		if (!butterfly.second)
		{
			return;
		}
	}
	for (auto& boss : m_Bosses)
	{
		if (!boss.second)
		{
			return;
		}
	}

	SendAIWave();
}
void SpawnerManagerComponent::SendAIWave()
{
	int idx{rand() % static_cast<int>(m_EnemyAIMoveSets.size())};
	int originalIndex{ idx };

	while (!m_EnemyAIMoveSets[idx]())
	{
		idx = (idx + 1) % static_cast<int>(m_EnemyAIMoveSets.size());
		if (idx == originalIndex)
		{
			return;
		}
	}
}

void SpawnerManagerComponent::SetupAIMoveSets()
{
	m_EnemyAIMoveSets.emplace_back([&]()
		{
			if (m_Bees.size() == 0 && m_Butterflies.size() == 0) return false;

			if (m_Bees.size() != 0)
			{
				int randomBee{ rand() % static_cast<int>(m_Bees.size()) };
				SendEnemyOnIndexOnBombingRun(m_Bees, randomBee);
			}
			if (m_Butterflies.size() != 0)
			{
				int randomButterfly{ rand() % static_cast<int>(m_Butterflies.size()) };
				SendEnemyOnIndexOnBombingRun(m_Butterflies, randomButterfly);
			}

			return true;
		});

	m_EnemyAIMoveSets.emplace_back([&]()
		{
			if (m_Bosses.size() == 0 && m_Butterflies.size() == 0) return false;

			if (m_Bosses.size() != 0)
			{
				int randomBoss{ rand() % static_cast<int>(m_Bosses.size()) };
				SendEnemyOnIndexOnBombingRun(m_Bosses, randomBoss);
			}
			if (m_Butterflies.size() > 1)
			{
				int randomButterfly1{ rand() % static_cast<int>(m_Butterflies.size()) };
				int randomButterfly2{ rand() % static_cast<int>(m_Butterflies.size()) };

				SendEnemyOnIndexOnBombingRun(m_Butterflies, randomButterfly1);
				SendEnemyOnIndexOnBombingRun(m_Butterflies, randomButterfly2);
			}
			else if (m_Butterflies.size() != 0)
			{
				int randomButterfly{ rand() % static_cast<int>(m_Butterflies.size()) };
				SendEnemyOnIndexOnBombingRun(m_Butterflies, randomButterfly);
			}
			
			return true;
		});

	m_EnemyAIMoveSets.emplace_back([&]()
		{
			if (m_Bosses.size() == 0) return false;

			int randomBoss{ rand() % static_cast<int>(m_Bosses.size()) };
			SendEnemyOnIndexOnTractorBeam(m_Bosses, randomBoss);

			return true;
		});
}

void SpawnerManagerComponent::SendEnemyOnIndexOnBombingRun(const std::list<std::pair<AE::GameObject*, bool>>& enemies, int index)
{
	auto enemyIt{ enemies.begin() };
	std::advance(enemyIt, index);
	if (auto fsmComp = (*enemyIt).first->GetComponent<FSMComponent>())
	{
		if (auto idle_01 = dynamic_cast<StatesEnemyBee::Idle*>(fsmComp->GetCurrentState()))
		{
			idle_01->ChangeToBombingRun();
		}
		else if (auto idle_02 = dynamic_cast<StatesEnemyButterfly::Idle*>(fsmComp->GetCurrentState()))
		{
			idle_02->ChangeToBombingRun();
		}
		else if (auto idle_03 = dynamic_cast<StatesEnemyBoss::Idle*>(fsmComp->GetCurrentState()))
		{
			idle_03->ChangeToBombingRun();
		}
	}
}

void SpawnerManagerComponent::SendEnemyOnIndexOnTractorBeam(const std::list<std::pair<AE::GameObject*, bool>>& enemies, int index)
{
	auto enemyIt{ enemies.begin() };
	std::advance(enemyIt, index);
	if (auto fsmComp = (*enemyIt).first->GetComponent<FSMComponent>())
	{
		if (auto idle = dynamic_cast<StatesEnemyBoss::Idle*>(fsmComp->GetCurrentState()))
		{
			idle->ChangeToTractorBeam();
		}
	}
}


