#pragma once

#include <memory>

#include "InputCommands.h"
#include "InputHelperStructs.h"

class Controller
{
public:
	Controller(int controllerIndex);
	~Controller();

	void ProcessInput();
	void BindAction(int key, InputType inputType, Command* const command);

private:

	class ControllerImpl;
	std::unique_ptr<ControllerImpl> m_pImpl;
	
};

enum ControllerXInput
{
	MYXINPUT_GAMEPAD_DPAD_UP         = 0x0001,
	MYXINPUT_GAMEPAD_DPAD_DOWN       = 0x0002,
	MYXINPUT_GAMEPAD_DPAD_LEFT       = 0x0004,
	MYXINPUT_GAMEPAD_DPAD_RIGHT      = 0x0008,
	MYXINPUT_GAMEPAD_START           = 0x0010,
	MYXINPUT_GAMEPAD_BACK            = 0x0020,
	MYXINPUT_GAMEPAD_LEFT_THUMB      = 0x0040,
	MYXINPUT_GAMEPAD_RIGHT_THUMB     = 0x0080,
	MYXINPUT_GAMEPAD_LEFT_SHOULDER   = 0x0100,
	MYXINPUT_GAMEPAD_RIGHT_SHOULDER  = 0x0200,
	MYXINPUT_GAMEPAD_A               = 0x1000,
	MYXINPUT_GAMEPAD_B               = 0x2000,
	MYXINPUT_GAMEPAD_X               = 0x4000,
	MYXINPUT_GAMEPAD_Y               = 0x8000
};

