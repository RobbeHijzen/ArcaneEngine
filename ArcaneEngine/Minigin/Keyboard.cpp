#include "Keyboard.h"

bool Keyboard::ProcessInput()
{
	const uint8_t* pKeyboardState = SDL_GetKeyboardState(nullptr);
	for (auto& inputBinding : m_InputBindings)
	{
		if (!inputBinding->command)
			continue;

		if (pKeyboardState[inputBinding->key])
		{
			inputBinding->command->Execute();
		}
	}

	return true;
}
