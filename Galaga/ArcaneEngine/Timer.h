#pragma once

#include <chrono>
#include <functional>

namespace AE
{
	class Timer
	{
	public:

		Timer(std::function<void(int)> itFunc, std::function<void()> destructionFunc, float itTime, int itNum, bool activateNow = false);
		bool Update();
		void CallDestructionFunc() { m_DestructionFunc(); }

	private:
		const std::function<void(int)> m_ItFunc{};
		const std::function<void()> m_DestructionFunc{};

		const float m_ItTime{};
		float m_CurrentItTime{};

		const int m_ItNum{};
		int m_CurrentItNum{};
	};
}

