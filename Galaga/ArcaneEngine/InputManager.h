#pragma once

#include <Windows.h>
#include <memory>
#include <type_traits>
#include <WinUser.h>

#include "Singleton.h"
#include "Command.h"
#include "InputHelperStructs.h"
#include "Controller.h"
#include "Keyboard.h"

namespace AE
{
	class InputManager final : public Singleton<InputManager>
	{
	public:

		InputManager();

		bool ProcessInput();

		void BindActionGP(int controllerIndex, int key, InputType inputType, std::unique_ptr<Command> command)
		{
			assert(controllerIndex < static_cast<int>(m_Controllers.size()));

			m_Controllers[controllerIndex]->BindAction(key, inputType, std::move(command));
		}

		void BindActionKB(SDL_Scancode key, InputType inputType, std::unique_ptr<Command> command)
		{
			m_Keyboard->BindAction(key, inputType, std::move(command));
		}

		void RemoveAllBindings();

	private:

		std::vector<std::unique_ptr<Controller>> m_Controllers{};
		const int m_ControllersAmount{ 2 };

		std::unique_ptr<Keyboard> m_Keyboard{};

	};
}



