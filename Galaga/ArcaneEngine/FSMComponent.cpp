#include "FSMComponent.h"

FSMComponent::FSMComponent(AE::GameObject* pParent, std::unique_ptr<AE::FSMState>&& startState)
	: BaseComponent(pParent)
	, m_CurrentState{std::move(startState)}
{
}

void FSMComponent::GameStart()
{
	m_CurrentState->OnEnter(GetOwner());
}

void FSMComponent::Update()
{
	auto owner{ GetOwner() };

	if (auto newState = m_CurrentState->Update(owner))
	{
		m_CurrentState->OnExit(owner);

		m_CurrentState.reset();
		m_CurrentState = std::move(newState);

		m_CurrentState->OnEnter(owner);
	}
}
