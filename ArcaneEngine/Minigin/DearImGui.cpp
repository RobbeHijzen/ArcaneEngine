#include "DearImGui.h"
#include <string>
#include <iostream>
#include <chrono>
#include <algorithm>

namespace MyImGui
{
	void RenderGraph(std::vector<float> graphValues, ImVec4 color)
	{

	}
}

void DearImGui::Initialize(SDL_Window* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();
}

void DearImGui::Render()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();


	ImGui::Begin("Excercise 1");

	m_ImGuiEx1.Render();

	ImGui::End();

	ImGui::Begin("Excercise 2");

	m_ImGuiEx2.Render();

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void DearImGuiEx1::Render()
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
		RenderGraphValues(m_CurrentGraphValues);
	}
}
std::vector<float> DearImGuiEx1::CalculateGraphValues()
{
	std::vector<std::vector<float>> allCalculatedTimes{};
	allCalculatedTimes.resize(m_SamplesAmount);

	for (int index{ 0 }; index < m_SamplesAmount; ++index)
	{
		allCalculatedTimes[index].resize(m_StepAmount);

		const int length{ 10'000'000 };
		int* arr{ new int[length] };


		for (int currentStepAmount{}; currentStepAmount < m_StepAmount; ++currentStepAmount)
		{
			int stepSize{static_cast<int>(pow(2, currentStepAmount))};

			auto start{ std::chrono::high_resolution_clock::now() };
			for (int i{}; i < length; i += stepSize)
			{
				arr[i] *= 2;
			}
			auto end{ std::chrono::high_resolution_clock::now() };
			auto time{ std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 10.f };

			allCalculatedTimes[index][currentStepAmount] = static_cast<float>(time);
		}
		delete[] arr;
	}

	//----Calculate Average Values-----
	std::vector<float> finalValues{};
	finalValues.resize(m_StepAmount);

	for (int stepIndex{}; stepIndex < m_StepAmount; ++stepIndex)
	{
		float averageStepTime{};

		// Keep track of min and max values, to subtract them later
		int currentMinSampleIndex{};
		int currentMaxSampleIndex{};

		// Go over every sample for a certain stepsize to calculate the average value
		for (int sampleIndex{}; sampleIndex < m_SamplesAmount; ++sampleIndex)
		{
			float currentTimeValue{ allCalculatedTimes[sampleIndex][stepIndex] };

			float currentMinValue{ allCalculatedTimes[currentMinSampleIndex][stepIndex] };
			float currentMaxValue{ allCalculatedTimes[currentMaxSampleIndex][stepIndex] };

			if (currentTimeValue < currentMinValue) currentMinSampleIndex = sampleIndex;
			else if (currentTimeValue > currentMaxValue) currentMaxSampleIndex = sampleIndex;
			averageStepTime += currentTimeValue;
		}
		// Remove the lowest and highest values
		float minSampleValue{ allCalculatedTimes[currentMinSampleIndex][stepIndex] };
		float maxSampleValue{ allCalculatedTimes[currentMaxSampleIndex][stepIndex] };
		averageStepTime -= (minSampleValue + maxSampleValue);

		// divide to calculate the average
		averageStepTime /= m_SamplesAmount - 2;

		finalValues[stepIndex] = averageStepTime;
	}

	return finalValues;
}
void DearImGuiEx1::RenderGraphValues(std::vector<float> graphValues)
{
	ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(0.0f, 1.0f, 1.0f, 1.0f));
	
	ImGui::PlotLines("", &graphValues[0], static_cast<int>(graphValues.size()), 
					 0, "Excercise 1", FLT_MAX, FLT_MAX,
					 ImVec2{ 160, 80 });
	ImGui::PopStyleColor();
}


void DearImGuiEx2::Render()
{
	ImGui::InputInt("#Samples", &m_SamplesAmount);
	if (m_SamplesAmount < m_MinimumSampleAmount) m_SamplesAmount = m_MinimumSampleAmount;

	// Normal
	if (ImGui::Button("Trash the Cache with GameObject3D"))
	{
		m_CurrentGraphValuesNormal = CalculateGraphValuesNormal();
		m_DrawGraphNormal = true;
	}
	if (m_DrawGraphNormal)
	{
		RenderGraphValues(m_CurrentGraphValuesNormal, ImVec4{1.f, 0.f, 1.f, 1.f});
	}

	// Alt
	if (ImGui::Button("Trash the Cache with GameObject3DAlt"))
	{
		m_CurrentGraphValuesAlt = CalculateGraphValuesAlt();
		m_DrawGraphAlt = true;
	}
	if (m_DrawGraphAlt)
	{
		RenderGraphValues(m_CurrentGraphValuesAlt, ImVec4{1.f, 1.f, 0.f, 1.f});
	}

	// Combined
	if (m_DrawGraphNormal && m_DrawGraphAlt)
	{
		ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4{1.f, 1.f, 1.f, 1.f});

		ImGui::PlotLines("", &m_CurrentGraphValuesNormal[0], static_cast<int>(m_CurrentGraphValuesAlt.size() * 2),
			0, NULL, -1.f, 1.f,
			ImVec2{ 160, 80 });
		ImGui::PopStyleColor();
	}
}
std::vector<float> DearImGuiEx2::CalculateGraphValuesNormal()
{
	std::vector<std::vector<float>> allCalculatedTimes{};
	allCalculatedTimes.resize(m_SamplesAmount);

	for (int index{ 0 }; index < m_SamplesAmount; ++index)
	{
		allCalculatedTimes[index].resize(m_StepAmount);

		const int length{ 10'000'000 };
		TestObjectNormal* arr{ new TestObjectNormal[length] };


		for (int currentStepAmount{}; currentStepAmount < m_StepAmount; ++currentStepAmount)
		{
			int stepSize{ static_cast<int>(pow(2, currentStepAmount)) };

			auto start{ std::chrono::high_resolution_clock::now() };
			for (int i{}; i < length; i += stepSize)
			{
				arr[i].id *= 2;
			}
			auto end{ std::chrono::high_resolution_clock::now() };
			auto time{ std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 10.f };

			allCalculatedTimes[index][currentStepAmount] = static_cast<float>(time);
		}
		delete[] arr;
	}

	//----Calculate Average Values-----
	std::vector<float> finalValues{};
	finalValues.resize(m_StepAmount);

	for (int stepIndex{}; stepIndex < m_StepAmount; ++stepIndex)
	{
		float averageStepTime{};

		// Keep track of min and max values, to subtract them later
		int currentMinSampleIndex{};
		int currentMaxSampleIndex{};

		// Go over every sample for a certain stepsize to calculate the average value
		for (int sampleIndex{}; sampleIndex < m_SamplesAmount; ++sampleIndex)
		{
			float currentTimeValue{ allCalculatedTimes[sampleIndex][stepIndex] };

			float currentMinValue{ allCalculatedTimes[currentMinSampleIndex][stepIndex] };
			float currentMaxValue{ allCalculatedTimes[currentMaxSampleIndex][stepIndex] };

			if (currentTimeValue < currentMinValue) currentMinSampleIndex = sampleIndex;
			else if (currentTimeValue > currentMaxValue) currentMaxSampleIndex = sampleIndex;
			averageStepTime += currentTimeValue;
		}
		// Remove the lowest and highest values
		float minSampleValue{ allCalculatedTimes[currentMinSampleIndex][stepIndex] };
		float maxSampleValue{ allCalculatedTimes[currentMaxSampleIndex][stepIndex] };
		averageStepTime -= (minSampleValue + maxSampleValue);

		// divide to calculate the average
		averageStepTime /= m_SamplesAmount - 2;

		finalValues[stepIndex] = averageStepTime;
	}

	return finalValues;
}
std::vector<float> DearImGuiEx2::CalculateGraphValuesAlt()
{
	std::vector<std::vector<float>> allCalculatedTimes{};
	allCalculatedTimes.resize(m_SamplesAmount);

	for (int index{ 0 }; index < m_SamplesAmount; ++index)
	{
		allCalculatedTimes[index].resize(m_StepAmount);

		const int length{ 10'000'000 };
		TestObjectAlt* arr{ new TestObjectAlt[length] };


		for (int currentStepAmount{}; currentStepAmount < m_StepAmount; ++currentStepAmount)
		{
			int stepSize{ static_cast<int>(pow(2, currentStepAmount)) };

			auto start{ std::chrono::high_resolution_clock::now() };
			for (int i{}; i < length; i += stepSize)
			{
				arr[i].id *= 2;
			}
			auto end{ std::chrono::high_resolution_clock::now() };
			auto time{ std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 10.f };

			allCalculatedTimes[index][currentStepAmount] = static_cast<float>(time);
		}
		delete[] arr;
	}

	//----Calculate Average Values-----
	std::vector<float> finalValues{};
	finalValues.resize(m_StepAmount);

	for (int stepIndex{}; stepIndex < m_StepAmount; ++stepIndex)
	{
		float averageStepTime{};

		// Keep track of min and max values, to subtract them later
		int currentMinSampleIndex{};
		int currentMaxSampleIndex{};

		// Go over every sample for a certain stepsize to calculate the average value
		for (int sampleIndex{}; sampleIndex < m_SamplesAmount; ++sampleIndex)
		{
			float currentTimeValue{ allCalculatedTimes[sampleIndex][stepIndex] };

			float currentMinValue{ allCalculatedTimes[currentMinSampleIndex][stepIndex] };
			float currentMaxValue{ allCalculatedTimes[currentMaxSampleIndex][stepIndex] };

			if (currentTimeValue < currentMinValue) currentMinSampleIndex = sampleIndex;
			else if (currentTimeValue > currentMaxValue) currentMaxSampleIndex = sampleIndex;
			averageStepTime += currentTimeValue;
		}
		// Remove the lowest and highest values
		float minSampleValue{ allCalculatedTimes[currentMinSampleIndex][stepIndex] };
		float maxSampleValue{ allCalculatedTimes[currentMaxSampleIndex][stepIndex] };
		averageStepTime -= (minSampleValue + maxSampleValue);

		// divide to calculate the average
		averageStepTime /= m_SamplesAmount - 2;

		finalValues[stepIndex] = averageStepTime;
	}

	return finalValues;
}
void DearImGuiEx2::RenderGraphValues(std::vector<float> graphValues, ImVec4 color)
{
	ImGui::PushStyleColor(ImGuiCol_PlotLines, color);

	ImGui::PlotLines("", &graphValues[0], static_cast<int>(graphValues.size()),
					 0, NULL, FLT_MAX, FLT_MAX,
					 ImVec2{ 160, 80 });
	ImGui::PopStyleColor();
}

