#include "StatesEnemyBee.h"

#include "StateHelpers.h"
#include "MyTime.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Observers.h"
#include "ShootComponent.h"
#include "StatesEnemy.h"
#include "TimeManager.h"

//****
// Idle
//****
std::unique_ptr<AE::FSMState> StatesEnemyBee::Idle::Update(AE::GameObject* gameObject)
{
	if (m_ChangeToBombingRun)
	{
		auto nextState{ std::make_unique<BombingRun>(m_StartingPos) };

		std::list<EnemySeekInfo> seekInfos{};
		seekInfos.emplace_back(EnemySeekInfo{ EnemySeekTypes::Circle, gameObject->GetWorldTransform().GetPosition() });

		return std::move(std::make_unique<StatesEnemy::Moving>(std::move(nextState), seekInfos));
	}

	return nullptr;
}

//****
// BombingRun
//****
void StatesEnemyBee::BombingRun::OnEnter(AE::GameObject* gameObject)
{
	AE::TimeManager::GetInstance().SetTimer([gameObject](int)
		{
			if (auto shootComp = gameObject->GetComponent<ShootComponent>())
			{
				shootComp->FireBullet();
			}
		}, m_ShootDelay, m_ShootAmount, true);

}
std::unique_ptr<AE::FSMState> StatesEnemyBee::BombingRun::Update(AE::GameObject* gameObject)
{
	std::list<EnemySeekInfo> seekInfo{};

	// Fill the seekInfo

	// Stage 1
	glm::vec2 pos{ gameObject->GetWorldTransform().GetPosition() };
	bool leftHalf{ pos.x < WINDOW_WIDTH / 2.f };
	glm::vec2 stage1Pos{ pos.x + (leftHalf ? m_Stage1Offset.x : -m_Stage1Offset.x), pos.y + m_Stage1Offset.y };
	seekInfo.emplace_back(EnemySeekInfo{ EnemySeekTypes::Straight, stage1Pos });

	// Stage 2
	glm::vec2 stage2Pos{stage1Pos.x, m_Stage2Height };
	seekInfo.emplace_back(EnemySeekInfo{ EnemySeekTypes::Straight, stage2Pos });

	// Stage 3
	glm::vec2 stage3Pos{ stage2Pos.x + (leftHalf ? -WINDOW_WIDTH / 4.f : WINDOW_WIDTH / 4.f), m_Stage3Height };
	seekInfo.emplace_back(EnemySeekInfo{ EnemySeekTypes::Straight, stage3Pos });

	// Stage 4
	glm::vec2 stage4Pos{ stage3Pos.x + (leftHalf ? -WINDOW_WIDTH / 4.f : WINDOW_WIDTH / 4.f), m_Stage4Height };
	seekInfo.emplace_back(EnemySeekInfo{ EnemySeekTypes::Straight, stage4Pos });

	// Stage 5
	glm::vec2 stage5Pos{ m_IdlePos };
	seekInfo.emplace_back(EnemySeekInfo{ EnemySeekTypes::Straight, stage5Pos });


	auto nextState{ std::make_unique<Idle>(m_IdlePos) };
	return std::move(std::make_unique<StatesEnemy::Moving>(std::move(nextState), seekInfo));
}
