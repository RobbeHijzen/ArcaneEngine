#include "ObserverManager.h"

#include "Observers.h"

using namespace ObserverPattern;

void ObserverManager::AddObserver(Observer* observer)
{
	m_Observers.emplace_back(observer);
}

void ObserverManager::RemoveObserver(Observer* observer)
{
	m_Observers.erase(std::find_if(m_Observers.begin(), m_Observers.end(), [&](const std::unique_ptr<Observer>& up)
	{
		return up.get() == observer;
	}));
}
