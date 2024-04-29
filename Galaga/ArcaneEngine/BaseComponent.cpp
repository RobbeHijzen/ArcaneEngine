#include "BaseComponent.h"

using namespace AE;

BaseComponent::BaseComponent(GameObject* pParent)
	: m_Owner{ pParent }
{
}
