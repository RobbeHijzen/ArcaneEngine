#pragma once

#include <vector>
#include <memory>

#include "Singleton.h"


namespace ObserverPattern
{
	class Observer;

	class ObserverManager final : public Singleton<ObserverManager>
	{
	public:

		void AddObserver(Observer* observer);
		void RemoveObserver(Observer* observer);

	private:

		std::vector<std::unique_ptr<Observer>> m_Observers;

	};
}