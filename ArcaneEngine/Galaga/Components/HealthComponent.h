#pragma once

#include "BaseComponent.h"

class HealthComponent : public BaseComponent
{
public:

	HealthComponent(GameObject* pParent, int maxHealth)
		: BaseComponent(pParent)
		, m_Health{maxHealth}
	{
	}


	void KillObject();
	int GetHealth() const { return m_Health; }

private:

	int m_Health{ 3 };
	bool m_HasDied{ false };

};

