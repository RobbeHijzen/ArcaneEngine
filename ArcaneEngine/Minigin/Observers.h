#pragma once

#include <iostream>
#include <vector>
#include "ObserverEvents.h"
#include "ObserverManager.h"

class GameObject;

namespace ObserverPattern
{
	class Subject;

	class Observer
	{
	public:

		Observer()
		{
			ObserverManager::GetInstance().AddObserver(this);
		}
		// Implemented in cpp file
		virtual ~Observer();

		Observer(const Observer& other) = delete;
		Observer(Observer&& other) = delete;
		Observer& operator=(const Observer& other) = delete;
		Observer& operator=(Observer&& other) = delete;


		virtual void Notify(Event event, GameObject* gameObject) = 0;

		void AddNewSubject(Subject* subject)
		{
			m_Subjects.emplace_back(subject);
		}
		void RemoveSubject(Subject* subject)
		{
			m_Subjects.erase(std::find(m_Subjects.begin(), m_Subjects.end(), subject));

			if (m_Subjects.size() == 0 && !m_IsDestroyed)
			{
				ObserverManager::GetInstance().RemoveObserver(this);
			}
		}	

	private:

		std::vector<Subject*> m_Subjects{};
		bool m_IsDestroyed{ false };
	};


	class PrintObserver : public Observer
	{
	public:

		virtual void Notify(Event event, GameObject* gameObject) override;
	};
}

