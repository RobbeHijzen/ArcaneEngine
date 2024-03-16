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

