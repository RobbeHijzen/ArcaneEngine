#pragma once

#include "BaseComponent.h"
#include "FSMState.h"
#include "GameObject.h"

#include <memory>

class FSMComponent : public AE::BaseComponent
{
public:
	FSMComponent(AE::GameObject* pParent, std::unique_ptr<AE::FSMState>&& startState);

	virtual void GameStart() override;
	virtual void Update() override;
		 
	AE::FSMState* GetCurrentState() const { return m_CurrentState.get(); }

private:

	std::unique_ptr<AE::FSMState> m_CurrentState;
};

