#pragma once

#include "BaseComponent.h"

class MovementComponent : public AE::BaseComponent
{
public:

	MovementComponent(AE::GameObject* pParent)
		: BaseComponent(pParent)
	{
	}

	void Move(glm::vec2 amount);

	void SetCanMove(bool canMove) { m_CanMove = canMove; }

private:

	bool m_CanMove{ true };

};

