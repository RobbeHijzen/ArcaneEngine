#pragma once

#include <memory>

#include "Command.h"
#include "InputHelperStructs.h"
namespace AE
{
	class Controller
	{
	public:
		Controller(int controllerIndex);
		~Controller();

		void ProcessInput();
		void BindAction(int key, InputType inputType, std::unique_ptr<Command> command);

	private:

		class ControllerImpl;
		std::unique_ptr<ControllerImpl> m_pImpl;

	};

	enum ControllerXInput
	{
		INPUT_GAMEPAD_DPAD_UP = 0x0001,
		INPUT_GAMEPAD_DPAD_DOWN = 0x0002,
		INPUT_GAMEPAD_DPAD_LEFT = 0x0004,
		INPUT_GAMEPAD_DPAD_RIGHT = 0x0008,
		INPUT_GAMEPAD_START = 0x0010,
		INPUT_GAMEPAD_BACK = 0x0020,
		INPUT_GAMEPAD_LEFT_THUMB = 0x0040,
		INPUT_GAMEPAD_RIGHT_THUMB = 0x0080,
		INPUT_GAMEPAD_LEFT_SHOULDER = 0x0100,
		INPUT_GAMEPAD_RIGHT_SHOULDER = 0x0200,
		INPUT_GAMEPAD_A = 0x1000,
		INPUT_GAMEPAD_B = 0x2000,
		INPUT_GAMEPAD_X = 0x4000,
		INPUT_GAMEPAD_Y = 0x8000
	};
}
