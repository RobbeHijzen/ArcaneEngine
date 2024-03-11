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
	void Render() const;

private:
	const int m_StepAmount{ 10 };

	mutable int m_SamplesAmount{ 10 };
	const int m_MinimumSampleAmount{ 4 };

	mutable std::vector<float> m_CurrentGraphValues{};
	mutable bool m_DrawGraph{ false };

	std::vector<float> CalculateGraphValues() const;
};

class DearImGuiEx2
{
public:
	void Render() const;

private:
	const int m_StepAmount{ 10 };

	mutable int m_SamplesAmount{ 5 };
	const int m_MinimumSampleAmount{ 4 };

	mutable std::vector<float> m_CurrentGraphValuesNormal{};
	mutable bool m_DrawGraphNormal{ false };

	mutable std::vector<float> m_CurrentGraphValuesAlt{};
	mutable bool m_DrawGraphAlt{ false };

	mutable PlotConfig m_CombinedConfig{};
	mutable bool m_FirstFrameButton{};

	std::vector<float> CalculateGraphValuesNormal() const;
	std::vector<float> CalculateGraphValuesAlt() const;

};



class DearImGui
{
public:
	DearImGui() = default;

	void Initialize(SDL_Window* window);
	void Render() const;

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

