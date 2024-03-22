#pragma once

#include <vector>
#include <memory>
#include "Observers.h"
#include "ObserverEvents.h"

class GameObject;

namespace ObserverPattern
{
	class Subject
	{
	public:

		Subject() = default;

		void AddObserver(Observer* newObserver)
		{
			m_Observers.emplace_back(newObserver);
		}
		void RemoveObserver(Observer* observer)
		{
			m_Observers.erase(std::find_if(m_Observers.begin(), m_Observers.end(), [&](const std::unique_ptr<Observer>& up)
			{
				return observer == up.get();
			}));
		}

		void Notify(Event event, GameObject* gameObject)
		{
			for (auto& observer : m_Observers)
			{
				observer->Notify(event, gameObject);
			}
		}

	private:

		std::vector<std::unique_ptr<Observer>> m_Observers{};
	};

}

