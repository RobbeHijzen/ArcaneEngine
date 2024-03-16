#include "ControllerImpl.h"


Controller::Controller(int controllerIndex)
	: m_pImpl{std::make_unique<ControllerImpl>(controllerIndex)}
{
}

Controller::~Controller()
{
}

void Controller::ProcessInput()
{
	m_pImpl->ProcessInput();
}


void Controller::BindAction(int key, InputType inputType, Command* const command)
{
	m_pImpl->BindAction(key, inputType, command);
}

