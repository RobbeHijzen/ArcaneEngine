#include "HealthComponent.h"
#include "GameObject.h"
#include "ObserverEvents.h"

void HealthComponent::KillObject()
{
	--m_Health;
	
	GetOwner()->NotifyAll(AE::Event::PlayerDied);

}