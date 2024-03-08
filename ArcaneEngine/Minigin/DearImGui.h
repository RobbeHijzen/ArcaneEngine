#pragma once

#include <SDL.h>
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

class DearImGui
{
public:
	DearImGui() = default;

	void Initialize(SDL_Window* window);
	void Render() const;

private:


};

