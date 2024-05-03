#include <SDL.h>
#include <iostream>
#include "InputManager.h"

using namespace AE;

InputManager::InputManager()
{
	m_Keyboard = std::make_unique<Keyboard>();

	m_Controllers.resize(m_ControllersAmount);
	for (int index{}; index < m_ControllersAmount; ++index)
	{
		m_Controllers[index] = std::make_unique<Controller>(index);
	}
	
}

bool InputManager::ProcessInput()
{
	// Input Management
	if (!m_Keyboard->ProcessInput())
	{
		return false;
	}

	for (auto& controller : m_Controllers)
	{
		controller->ProcessInput();
	}
	//------------------
	
	return true;
}

void AE::InputManager::RemoveAllBindings()
{
	m_Keyboard->RemoveAllBindings();

	for (auto& controller : m_Controllers)
	{
		controller->RemoveAllBindings();
	}
}



