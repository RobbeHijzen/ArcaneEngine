#pragma once

#include "Singleton.h"
#include "Timer.h"
#include <chrono>
#include <functional>

namespace AE
{
	class TimeManager : public Singleton<TimeManager>
	{
	public:

		int SetTimer(std::function<void(int)> itFunc, float itTime, int itNum, bool activateNow = false, std::function<void()> endFunc = []() {});
		void ClearTimer(int handle);
		void Update();

		void ClearAllTimers()
		{
			m_Timers.clear();
			m_AllExistedValues.clear();
		}
		bool DoesTimerExist(int handle);

	private:
		std::list<std::pair<std::unique_ptr<Timer>, int>> m_Timers{};

		int GetRandomValue();
		bool HasValueExisted(int value);
		std::list<int> m_AllExistedValues{};
	};
}

