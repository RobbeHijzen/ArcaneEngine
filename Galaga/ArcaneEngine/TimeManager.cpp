#include "TimeManager.h"

int AE::TimeManager::SetTimer(std::function<void(int)> itFunc, float itTime, int itNum, std::function<void()> endFunc)
{
	m_Timers.emplace_back(std::move(std::make_unique<Timer>(itFunc, itTime, itNum)), endFunc);
	return static_cast<int>(m_Timers.size()) - 1;
}

void AE::TimeManager::ClearTimer(int index)
{
	if (index < 0 || index >= m_Timers.size()) return;

	m_Timers[index].second();

	auto it{ m_Timers.begin() };
	std::advance(it, index);
	m_Timers.erase(it);
}

void AE::TimeManager::Update()
{
	for (int index{}; index < m_Timers.size(); ++index)
	{
		auto timer{m_Timers[index].first.get()};

		if (!timer->Update())
		{
			m_Timers[index].second();

			auto it{ m_Timers.begin() };
			std::advance(it, index);
			m_Timers.erase(it);
			--index;
		}
	}
}
