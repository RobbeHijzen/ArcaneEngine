#pragma once
#include "Subject.h"

namespace AE
{
	class GameInstance
	{
	public:
		virtual ~GameInstance() {}

		void NotifyAll(Event event) { m_pSubject->NotifyAll(event, nullptr); }
		void AddObserver(std::unique_ptr<Observer> observer)
		{
			m_pSubject->AddObserver(std::move(observer));
		}
		void RemoveObserver(Observer* observer)
		{
			m_pSubject->RemoveObserver(observer);
		}
		void ClearObservers()
		{
			m_pSubject->ClearObservers();
		}

	private:
		std::unique_ptr<Subject> m_pSubject{ std::make_unique<Subject>() };
	};

}