#pragma once
#include "BaseComponent.h"
#include "HitboxComponent.h"

class NoOffScreenComponent : public AE::BaseComponent
{
public:

	NoOffScreenComponent(AE::GameObject* pParent, HitboxComponent* hitboxComp)
		: BaseComponent(pParent)
		, m_HitboxComp{ hitboxComp }
	{
	}

	virtual void Update() override;

private:

	HitboxComponent* m_HitboxComp{};
};
