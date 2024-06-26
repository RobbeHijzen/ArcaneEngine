#pragma once

#include <list>
#include <memory>
#include "Observer.h"
#include "ObserverEvents.h"

class GameObject;

namespace AE
{
	class Subject
	{
	public:

		Subject() = default;

		void AddObserver(std::unique_ptr<Observer> newObserver)
		{
			m_Observers.emplace_back(std::move(newObserver));
		}
		void RemoveObserver(Observer* observer)
		{
			for (auto it{ m_Observers.begin() }; it != m_Observers.end(); ++it)
			{
				if (observer == (*it).get())
				{
					m_Observers.erase(it);
					return;
				}
			}
		}
		void ClearObservers()
		{
			m_Observers.clear();
		}

		void NotifyAll(Event event, GameObject* gameObject)
		{
			for (auto& observer : m_Observers)
			{
				observer->OnNotify(event, gameObject);
			}
		}

	private:

		std::vector<std::unique_ptr<Observer>> m_Observers{};
	};

}

