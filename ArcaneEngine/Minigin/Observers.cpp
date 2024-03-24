#include "Observers.h"

#include "Subject.h"
#include "GameObject.h"

using namespace ObserverPattern;


ObserverPattern::Observer::~Observer()
{
	m_IsDestroyed = true;

	for (Subject* subject : m_Subjects)
	{
		subject->RemoveObserver(this);
	}
}



void PrintObserver::Notify(Event event, GameObject*)
{
	switch (event)
	{
		case Event::PrintTest:
		{
			std::cout << "Test Printed\n";
		}
	}
}
