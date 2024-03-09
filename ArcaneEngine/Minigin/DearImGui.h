#pragma once

#include <SDL.h>
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include <vector>

class DearImGuiEx1
{
public:
	void Render();

private:
	int m_StepAmount{ 10 };

	int m_SamplesAmount{ 10 };
	int m_MinimumSampleAmount{ 4 };
	std::vector<float> m_CurrentGraphValues{};
	bool m_DrawGraph{ false };


	std::vector<float> CalculateGraphValues();
	void RenderGraphValues(std::vector<float> graphValues);
};

class DearImGuiEx2
{
public:
	void Render();

private:
	int m_StepAmount{ 10 };

	int m_SamplesAmount{ 50 };
	int m_MinimumSampleAmount{ 4 };

	std::vector<float> m_CurrentGraphValuesNormal{};
	bool m_DrawGraphNormal{ false };

	std::vector<float> m_CurrentGraphValuesAlt{};
	bool m_DrawGraphAlt{ false };

	std::vector<float> CalculateGraphValuesNormal();
	std::vector<float> CalculateGraphValuesAlt();

	void RenderGraphValues(std::vector<float> graphValues, ImVec4 color);

};



class DearImGui
{
public:
	DearImGui() = default;

	void Initialize(SDL_Window* window);
	void Render();

private:

	DearImGuiEx1 m_ImGuiEx1{};
	DearImGuiEx2 m_ImGuiEx2{};
	
};

struct TestTransform
{
	float matrix[16] = { 1, 0, 0, 0,
						0, 1, 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1 };
};

class TestObjectNormal
{
public:
	TestTransform local{};
	int id{};
};
class TestObjectAlt
{
public:
	TestTransform* local{};
	int id{};
};

