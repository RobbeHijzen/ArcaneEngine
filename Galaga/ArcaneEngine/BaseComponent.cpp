#include "BaseComponent.h"
#include "GameObject.h"

using namespace AE;

BaseComponent::BaseComponent(GameObject* pParent)
	: m_Owner{ pParent }
{
}

Transform BaseComponent::GetWorldTransform() const
{
	return GetOwner()->GetWorldTransform() + m_LocalTransform;
}
