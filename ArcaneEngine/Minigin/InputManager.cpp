#include <SDL.h>
#include <iostream>
#include "InputManager.h"

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
	//SDL_Event e;
	//while (SDL_PollEvent(&e))
	//{
	//	if (e.type == SDL_QUIT)
	//	{
	//		return false;
	//	}
	//}

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



