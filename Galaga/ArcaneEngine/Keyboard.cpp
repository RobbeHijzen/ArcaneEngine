#include "Keyboard.h"

Keyboard::Keyboard()
{
    m_CurrentKeyStates.resize(SDL_NUM_SCANCODES);
    m_PreviousKeyStates.resize(SDL_NUM_SCANCODES);
}

bool Keyboard::ProcessInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            return false;
        }

        switch (event.type)
        {
            case SDL_KEYDOWN:
            {
                HandleKeyDown(event.key.keysym.scancode);
                break;
            }
            case SDL_KEYUP:
            {
                HandleKeyUp(event.key.keysym.scancode);
                break;
            }

        default:
            break;
        }
    }

    for (auto& inputBinding : m_InputBindings)
    {
        if (!inputBinding->command)
            continue;

        switch (inputBinding->inputType)
        {
            case InputType::IsPressed:
            {
                if (IsKeyPressed(inputBinding->key))
                {
                    inputBinding->command->Execute();
                }
                break;
            }
            case InputType::IsDownThisFrame:
            {
                if (IsKeyDownThisFrame(inputBinding->key))
                {
                    inputBinding->command->Execute();
                }
                break;
            }
            case InputType::IsUpThisFrame:
            {
                if (IsKeyUpThisFrame(inputBinding->key))
                {
                    inputBinding->command->Execute();
                }
                break;
            }
        }
    }

    m_PreviousKeyStates = m_CurrentKeyStates;

    return true;
}

bool Keyboard::IsKeyPressed(SDL_Scancode key)
{
    return m_CurrentKeyStates[key];
}

bool Keyboard::IsKeyDownThisFrame(SDL_Scancode key)
{
    return m_CurrentKeyStates[key] && !m_PreviousKeyStates[key];
}

bool Keyboard::IsKeyUpThisFrame(SDL_Scancode key)
{
    return !m_CurrentKeyStates[key] && m_PreviousKeyStates[key];
}

void Keyboard::HandleKeyDown(SDL_Scancode key)
{
    m_CurrentKeyStates[key] = true;
}

void Keyboard::HandleKeyUp(SDL_Scancode key)
{
    m_CurrentKeyStates[key] = false;
}