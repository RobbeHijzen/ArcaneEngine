#include "TimeManager.h"

int AE::TimeManager::SetTimer(std::function<void(int)> itFunc, float itTime, int itNum, bool activateNow, std::function<void()> endFunc)
{
	int randomHandle{ GetRandomValue() };
	m_Timers.emplace_back(std::move(std::make_unique<Timer>(itFunc, endFunc, itTime, itNum, activateNow)),
						  randomHandle);
	return randomHandle;
}

void AE::TimeManager::ClearTimer(int handle)
{	
	auto it{ m_Timers.begin() };
	while(it != m_Timers.end())
	{
		if ((*it).second == handle)
		{
			(*it).first->CallDestructionFunc();
			it = m_Timers.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void AE::TimeManager::Update()
{
	std::vector<int> toClearTimers{};
	for (auto& timer : m_Timers)
	{
		if (!timer.first->Update())
		{
			toClearTimers.emplace_back(timer.second);
		}
	}
	for(int handle : toClearTimers)
		ClearTimer(handle);
}

bool AE::TimeManager::DoesTimerExist(int handle)
{
	for (const auto& timer : m_Timers)
	{
		if (timer.second == handle)
		{
			return true;
		}
	}
	return false;
}

int AE::TimeManager::GetRandomValue()
{
	int randomValue{ rand() };
	while (HasValueExisted(randomValue) || randomValue == 0)
	{
		randomValue = rand();
	}
	m_AllExistedValues.emplace_back(randomValue);
	return randomValue;
}

bool AE::TimeManager::HasValueExisted(int value)
{
	for (int i : m_AllExistedValues)
	{
		if (i == value) return true;
	}
	return false;
}
