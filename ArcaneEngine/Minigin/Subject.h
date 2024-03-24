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

		~Subject()
		{
			for (auto& observer : m_Observers)
			{
				observer->RemoveSubject(this);
			}
		}

		void AddObserver(Observer* newObserver)
		{
			m_Observers.emplace_back(newObserver);

			newObserver->AddNewSubject(this);
		}
		void RemoveObserver(Observer* observer)
		{
			m_Observers.erase(std::find(m_Observers.begin(), m_Observers.end(), observer));

			observer->RemoveSubject(this);
		}

		void Notify(Event event, GameObject* gameObject)
		{
			for (auto& observer : m_Observers)
			{
				observer->Notify(event, gameObject);
			}
		}

	private:

		std::vector<Observer*> m_Observers{};
	};

}

