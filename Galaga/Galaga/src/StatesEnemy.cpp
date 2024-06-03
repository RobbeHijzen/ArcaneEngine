#include "StatesEnemy.h"
#include "MyTime.h"

//****
// Spawning
//****
void StatesEnemy::Spawning::OnEnter(AE::GameObject*)
{
	m_CurrentSeekInfo = m_SeekInfo.front();
}

std::unique_ptr<AE::FSMState> StatesEnemy::Spawning::Update(AE::GameObject* gameObject)
{
	MoveAccordingToSeekInfo(m_CurrentSeekInfo, gameObject);

	if (AE::StateHelpers::HasReached(m_CurrentSeekInfo.seekPos, gameObject))
	{
		if (m_SeekInfo.size() > 1)
		{
			m_SeekInfo.pop();
			m_CurrentSeekInfo = m_SeekInfo.front();
		}
		else
		{
			return std::move(m_NextState);
		}
	}

	return nullptr;
}

void StatesEnemy::Spawning::MoveAccordingToSeekInfo(EnemySeekInfo seekInfo, AE::GameObject* gameObject)
{
	switch (seekInfo.seekType)
	{
		case EnemySeekTypes::Straight:
		{
			glm::vec2 dir = AE::StateHelpers::GetSeekDirection(seekInfo.seekPos, gameObject);
			glm::vec2 addedPosition{ dir * m_MoveSpeed * AE::Time::GetInstance().GetDeltaTime() };
			gameObject->AddLocalTransform(AE::Transform{ addedPosition });
			break;
		}
		case EnemySeekTypes::Circle:
		{

			break;
		}

		default: break;
	}
}
