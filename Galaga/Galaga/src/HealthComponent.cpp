#include "HealthComponent.h"
#include "GameObject.h"
#include "ObserverEvents.h"

void HealthComponent::KillObject()
{
	--m_Health;
	GetOwner()->NotifyAll(AE::Event::ObjectLostHealth);

	if (m_Health <= 0)
	{
		GetOwner()->NotifyAll(AE::Event::ObjectDied);
	}
}
