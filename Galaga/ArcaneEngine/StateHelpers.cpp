#include "StateHelpers.h"

bool AE::StateHelpers::HasReachedPoint(glm::vec2 seekPos, AE::GameObject* gameObject, float acceptanceRadius)
{
	glm::vec2 difference{ seekPos - glm::vec2{gameObject->GetLocalTransform().GetPosition()} };
	float distanceSq{ glm::sqrt(difference.x * difference.x + difference.y * difference.y) };

	return distanceSq <= acceptanceRadius * acceptanceRadius;
}

glm::vec2 AE::StateHelpers::GetDirection(glm::vec2 seekPos, AE::GameObject* gameObject)
{
	glm::vec2 dir{ seekPos - glm::vec2{gameObject->GetLocalTransform().GetPosition()} };

	if (dir.x || dir.y)
	{
		return glm::normalize(dir);
	}
	return glm::vec2{};
}
