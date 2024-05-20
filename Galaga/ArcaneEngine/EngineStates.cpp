#include "EngineStates.h"
#include "MyTime.h"

void AE::States::Seek::OnEnter(AE::GameObject* gameObject)
{
	m_SeekDir = StateHelpers::GetSeekDirection(m_SeekPos, gameObject);
}
std::unique_ptr<AE::FSMState> AE::States::Seek::Update(AE::GameObject* gameObject)
{
	glm::vec2 addedPosition{ m_SeekDir * m_MoveSpeed * AE::Time::GetInstance().GetDeltaTime() };
	gameObject->AddLocalTransform(AE::Transform{ addedPosition });

	if (StateHelpers::HasReached(m_SeekPos, gameObject))
	{
		return std::move(m_NextState);
	}
	return nullptr;
}


glm::vec2 AE::StateHelpers::GetSeekDirection(glm::vec2 seekPos, AE::GameObject* gameObject)
{
	glm::vec2 dir{ seekPos - glm::vec2{gameObject->GetLocalTransform().GetPosition()} };

	if (dir.x || dir.y)
	{
		return glm::normalize(dir);
	}
	return glm::vec2{};
}

bool AE::StateHelpers::HasReached(glm::vec2 seekPos, AE::GameObject* gameObject, float acceptanceRadius)
{
	glm::vec2 difference{ seekPos - glm::vec2{gameObject->GetLocalTransform().GetPosition()} };
	float distanceSq{ glm::sqrt(difference.x * difference.x + difference.y * difference.y) };

	return distanceSq <= acceptanceRadius * acceptanceRadius;
}
