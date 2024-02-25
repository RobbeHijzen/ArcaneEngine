#include "BaseComponent.h"

BaseComponent::BaseComponent(std::weak_ptr<GameObject> gameObject)
	: m_pParentGameObject{ gameObject }
{
}
