#include "MovementComponent.h"

#include "Time.h"

MovementComponent::MovementComponent(GameObject* const parentGameObject, float movementSpeed)
	: BaseComponent(parentGameObject)
	, m_MovementSpeed{movementSpeed}
{

}

void MovementComponent::Move(glm::vec2 direction)
{
	glm::vec2 addedPosition{ direction * m_MovementSpeed * Time::GetInstance().GetDeltaTime()};
	GetOwner()->AddLocalTransform(Transform{ addedPosition });
}
