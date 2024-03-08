#include "DearImGui.h"
#include <string>
#include <iostream>

int NumericalInputFilter(ImGuiInputTextCallbackData* data) 
{
	if (data->EventChar < 256 && strchr("0123456789.-", (char)data->EventChar))
		return 0;
	return 1;
}

void DearImGui::Initialize(SDL_Window* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();
}

void DearImGui::Render() const
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	// ImGui commands here
	ImGui::Begin("Excercise 1");

	char samplesString[10]{'1', '0'};
	if (ImGui::InputText("#Samples", samplesString, sizeof(samplesString), ImGuiInputTextFlags_CallbackCharFilter, NumericalInputFilter))
	{
		int samplesAmount{ std::atoi(samplesString) };
		std::cout << samplesAmount << "\n";
	}


	ImGui::End();
	//

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
