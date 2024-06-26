#pragma once

#include "Singleton.h"
#include <chrono>

using namespace std::chrono;

namespace AE
{
	struct Time final : public Singleton<Time>
	{
	public:

		void Update();
		void Initialize(float fixedTimeStep, int msPerFrame);

		float GetDeltaTime() const { return m_DeltaTime; }
		float GetFixedTimeStep() const { return m_FixedTimeStep; }
		int GetMsPerFrame() const { return m_MsPerFrame; }
		float GetFps() const { return m_Fps; }
		auto GetCurrentTime() const { return m_CurrentTime; }

	private:

		float m_DeltaTime{};
		float m_FixedTimeStep{};
		int m_MsPerFrame{};
		int m_MicroSecondsPerFrame{};
		float m_Fps{};

		const float m_MaxDeltaTime{ 1.f };

		steady_clock::time_point m_LastTime{ high_resolution_clock::now() };
		steady_clock::time_point m_CurrentTime{};
	};
}
