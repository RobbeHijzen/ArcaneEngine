#include "Timer.h"
#include "MyTime.h"

AE::Timer::Timer(std::function<void(int)> itFunc, float itTime, int itNum, bool activateNow)
	: m_ItFunc{itFunc}
	, m_ItTime{itTime}
	, m_ItNum{itNum}
{
	if (activateNow)
	{
		m_ItFunc(m_CurrentItNum++);
	}
}

bool AE::Timer::Update()
{
	m_CurrentItTime += AE::Time::GetInstance().GetDeltaTime();
	while (m_CurrentItTime >= m_ItTime)
	{
		m_CurrentItTime -= m_ItTime;

		m_ItFunc(m_CurrentItNum++);
		if (m_CurrentItNum >= m_ItNum)
		{
			return false;
		}
	}

	return true;
}
