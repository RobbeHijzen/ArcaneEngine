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

		int SetTimer(std::function<void(int)> itFunc, float itTime, int itNum, std::function<void()> endFunc = []() {});
		void ClearTimer(int index);
		void Update();

	private:
		std::vector<std::pair<std::unique_ptr<Timer>, std::function<void()>>> m_Timers{};

	};
}

