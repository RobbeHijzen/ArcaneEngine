#pragma once

#include <chrono>
#include <functional>

namespace AE
{
	class Timer
	{
	public:

		Timer(std::function<void(int)> itFunc, float itTime, int itNum);
		bool Update();

	private:
		const std::function<void(int)> m_ItFunc{};

		const float m_ItTime{};
		float m_CurrentItTime{};

		const int m_ItNum{};
		int m_CurrentItNum{};
	};
}

