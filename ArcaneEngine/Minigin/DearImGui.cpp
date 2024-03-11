#include "DearImGui.h"
#include <string>
#include <iostream>
#include <chrono>
#include <algorithm>

namespace MyImGui
{
	static void RenderGraph(const std::vector<float>& graphValues, const ImVec4 color, const std::string& title)
	{
		ImGui::PushStyleColor(ImGuiCol_PlotLines, color);
		ImGui::PlotLines("", &graphValues[0], static_cast<int>(graphValues.size()),
						 0, title.c_str(), FLT_MAX, FLT_MAX,
						 ImVec2{ 160, 80 });
		ImGui::PopStyleColor();
	}

	static std::vector<float> TrashTheCache(auto* arr, int arrLength, int stepAmount)
	{
		std::vector<float> times{};
		times.resize(stepAmount);

		for (int currentStepAmount{}; currentStepAmount < stepAmount; ++currentStepAmount)
		{
			int stepSize{ static_cast<int>(pow(2, currentStepAmount)) };

			auto start{ std::chrono::high_resolution_clock::now() };
			for (int i{}; i < arrLength; i += stepSize)
			{
				arr[i] *= 2;
			}
			auto end{ std::chrono::high_resolution_clock::now() };
			auto time{ std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 10.f };

			times[currentStepAmount] = static_cast<float>(time);
		}

		return times;
	}

	static std::vector<float> CalculateAverageTimes(std::vector<std::vector<float>> allTimes, int stepAmount, int samplesAmount)
	{
		//----Calculate Average Values-----
		std::vector<float> finalValues{};
		finalValues.resize(stepAmount);

		for (int stepIndex{}; stepIndex < stepAmount; ++stepIndex)
		{
			float averageStepTime{};

			// Keep track of min and max values, to subtract them later
			int currentMinSampleIndex{};
			int currentMaxSampleIndex{};

			// Go over every sample for a certain stepsize to calculate the average value
			for (int sampleIndex{}; sampleIndex < samplesAmount; ++sampleIndex)
			{
				float currentTimeValue{ allTimes[sampleIndex][stepIndex] };

				float currentMinValue{ allTimes[currentMinSampleIndex][stepIndex] };
				float currentMaxValue{ allTimes[currentMaxSampleIndex][stepIndex] };

				if (currentTimeValue < currentMinValue) currentMinSampleIndex = sampleIndex;
				else if (currentTimeValue > currentMaxValue) currentMaxSampleIndex = sampleIndex;
				averageStepTime += currentTimeValue;
			}
			// Remove the lowest and highest values
			float minSampleValue{ allTimes[currentMinSampleIndex][stepIndex] };
			float maxSampleValue{ allTimes[currentMaxSampleIndex][stepIndex] };
			averageStepTime -= (minSampleValue + maxSampleValue);

			// divide to calculate the average
			averageStepTime /= samplesAmount - 2;

			finalValues[stepIndex] = averageStepTime;
		}

		return finalValues;
	}

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


	ImGui::Begin("Excercise 1");
	ImGui::SetWindowPos({ 100.f, 100.f }, ImGuiCond_FirstUseEver);
	m_ImGuiEx1.Render();
	ImGui::End();

	ImGui::Begin("Excercise 2");
	ImGui::SetWindowPos({ 500.f, 100.f }, ImGuiCond_FirstUseEver);
	ImGui::SetWindowSize({ 150.f, 300.f }, ImGuiCond_FirstUseEver);
	m_ImGuiEx2.Render();
	ImGui::End();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void DearImGuiEx1::Render() const
{
	ImGui::InputInt("#Samples", &m_SamplesAmount);
	if (m_SamplesAmount < m_MinimumSampleAmount) m_SamplesAmount = m_MinimumSampleAmount;

	if (ImGui::Button("Trash the Cache"))
	{
		m_CurrentGraphValues = CalculateGraphValues();
		m_DrawGraph = true;
	}
	if (m_DrawGraph)
	{
		MyImGui::RenderGraph(m_CurrentGraphValues, ImVec4{0.f, 1.f, 1.f, 1.f}, "Excercise 1");
	}
}
std::vector<float> DearImGuiEx1::CalculateGraphValues() const
{
	std::vector<std::vector<float>> allCalculatedTimes{};
	allCalculatedTimes.resize(m_SamplesAmount);

	for (int index{ 0 }; index < m_SamplesAmount; ++index)
	{
		const int length{ 10'000'000 };
		int* arr{ new int[length] };

		allCalculatedTimes[index] = MyImGui::TrashTheCache(arr, length, m_StepAmount);
		
		delete[] arr;
	}

	return MyImGui::CalculateAverageTimes(allCalculatedTimes, m_StepAmount, m_SamplesAmount);
}


void DearImGuiEx2::Render() const
{
	ImGui::InputInt("#Samples", &m_SamplesAmount);
	if (m_SamplesAmount < m_MinimumSampleAmount) m_SamplesAmount = m_MinimumSampleAmount;

	// Normal
	if (ImGui::Button("Trash the Cache with GameObject3D"))
	{
		m_CurrentGraphValuesNormal = CalculateGraphValuesNormal();
		m_DrawGraphNormal = true;
		m_FirstFrameButton = true;
	}
	if (m_DrawGraphNormal)
	{
		MyImGui::RenderGraph(m_CurrentGraphValuesNormal, ImVec4{1.f, 0.f, 1.f, 1.f}, "");
	}

	// Alt
	if (ImGui::Button("Trash the Cache with GameObject3DAlt"))
	{
		m_CurrentGraphValuesAlt = CalculateGraphValuesAlt();
		m_DrawGraphAlt = true;
		m_FirstFrameButton = true;
	}
	if (m_DrawGraphAlt)
	{
		MyImGui::RenderGraph(m_CurrentGraphValuesAlt, ImVec4{1.f, 1.f, 0.f, 1.f}, "");
	}

	// Combined Config
	if (m_FirstFrameButton && m_DrawGraphNormal && m_DrawGraphAlt)
	{
		m_FirstFrameButton = false;

		static const float* combinedGraphs[]{ m_CurrentGraphValuesNormal.data(), m_CurrentGraphValuesAlt.data() };
		static ImU32 colors[2] = { ImColor(255, 0, 255), ImColor(255, 255, 0) };

		PlotConfig config;
		config.values.count = m_StepAmount;
		config.values.ys_list = combinedGraphs;
		config.values.ys_count = 2;
		config.values.colors = colors;
		config.scale.min = 0;
		config.scale.max = 1.2f * std::max(*std::max_element(m_CurrentGraphValuesNormal.begin(), m_CurrentGraphValuesNormal.end()), *std::max_element(m_CurrentGraphValuesAlt.begin(), m_CurrentGraphValuesAlt.end()));
		config.grid_y.show = true;
		config.grid_y.size = 1000;
		config.frame_size = ImVec2(160, 120);

		m_CombinedConfig = config;
	}

	// Combined
	if (m_DrawGraphNormal && m_DrawGraphAlt)
	{
		Plot("Test", m_CombinedConfig);
	}
}
std::vector<float> DearImGuiEx2::CalculateGraphValuesNormal() const
{
	std::vector<std::vector<float>> allCalculatedTimes{};
	allCalculatedTimes.resize(m_SamplesAmount);

	for (int index{ 0 }; index < m_SamplesAmount; ++index)
	{
		const int length{ 10'000'000 };
		TestObjectNormal* arr{ new TestObjectNormal[length] };

		allCalculatedTimes[index] = MyImGui::TrashTheCache(arr, length, m_StepAmount);

		delete[] arr;
	}

	return MyImGui::CalculateAverageTimes(allCalculatedTimes, m_StepAmount, m_SamplesAmount);
}
std::vector<float> DearImGuiEx2::CalculateGraphValuesAlt() const
{
	std::vector<std::vector<float>> allCalculatedTimes{};
	allCalculatedTimes.resize(m_SamplesAmount);

	for (int index{ 0 }; index < m_SamplesAmount; ++index)
	{
		const int length{ 10'000'000 };
		TestObjectAlt* arr{ new TestObjectAlt[length] };

		allCalculatedTimes[index] = MyImGui::TrashTheCache(arr, length, m_StepAmount);

		delete[] arr;
	}

	return MyImGui::CalculateAverageTimes(allCalculatedTimes, m_StepAmount, m_SamplesAmount);
}

