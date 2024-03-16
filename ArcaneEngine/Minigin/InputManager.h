#pragma once

#include <Windows.h>
#include <Xinput.h>
#include <memory>
#include <type_traits>
#include <WinUser.h>

#include "Singleton.h"
#include "InputCommands.h"
#include "InputHelperStructs.h"
#include "Controller.h"
#include "Keyboard.h"


class InputManager final : public Singleton<InputManager>
{
public:

	InputManager();

	bool ProcessInput();

	void BindActionGP(int controllerIndex, int key, InputTypeGP inputType, Command* const command)
	{
		assert(controllerIndex < static_cast<int>(m_Controllers.size()));

		m_Controllers[controllerIndex]->BindAction(key, inputType, command);
	}

	void BindActionKB(SDL_Scancode key, auto command)
	{
		m_Keyboard->BindAction(key, command);
	}

private:

	std::vector<std::unique_ptr<Controller>> m_Controllers{};
	const int m_ControllersAmount{ 2 };

	std::unique_ptr<Keyboard> m_Keyboard{};
	
};



