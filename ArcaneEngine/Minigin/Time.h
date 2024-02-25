#pragma once

#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

class Time final: public Singleton<Time>
{
public:

	void SetDeltaTime(float deltaTime) { m_DeltaTime = deltaTime; }
	void SetFixedTimeStep(float fixedTimeStep) { m_FixedTimeStep = fixedTimeStep; }
	void SetMsPerFrame(int msPerFrame) { m_MsPerFrame = msPerFrame; }

	float GetDeltaTime() const { return m_DeltaTime; }
	float GetFixedTimeStep() const { return m_FixedTimeStep; }
	int GetMsPerFrame() const { return m_MsPerFrame; }

private:

	float m_DeltaTime{};
	float m_FixedTimeStep{};
	int m_MsPerFrame{};
};

