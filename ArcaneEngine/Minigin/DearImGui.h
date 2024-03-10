#pragma once

#include <SDL.h>
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include <vector>
#include "ImGuiPlot.h"

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

	PlotConfig m_CombinedConfig{};
	bool m_FirstFrameButton{};

	std::vector<float> CalculateGraphValuesNormal();
	std::vector<float> CalculateGraphValuesAlt();

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

	int operator*=(int other)
	{
		return id *= other;
	}
};
class TestObjectAlt
{
public:
	TestTransform* local{};
	int id{};

	int operator*=(int other)
	{
		return id *= other;
	}
};

