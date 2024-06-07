#include "MovementComponent.h"
#include "GameObject.h"

void MovementComponent::Move(glm::vec2 amount)
{
	if (m_CanMove)
	{
		GetOwner()->AddLocalTransform({ amount.x, amount.y });
	}
}
