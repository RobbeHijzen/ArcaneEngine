#include "Time.h"
#include <algorithm>

void Time::Update()
{
	m_CurrentTime = high_resolution_clock::now();
	m_DeltaTime =  std::clamp(duration<float>(m_CurrentTime - m_LastTime).count(), 0.f, m_MaxDeltaTime);
	m_Fps = 1.f / m_DeltaTime;
	m_LastTime = m_CurrentTime;
}

void Time::Initialize(float fixedTimeStep, float msPerFrame)
{
	m_FixedTimeStep = fixedTimeStep;
	m_MsPerFrame = msPerFrame;
	m_MicroSecondsPerFrame = static_cast<int>(m_MsPerFrame * 1000);
}
