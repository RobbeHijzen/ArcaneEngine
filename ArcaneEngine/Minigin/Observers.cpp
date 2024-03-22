#include "Observers.h"

#include "Subject.h"
#include "GameObject.h"

using namespace ObserverPattern;

PrintObserver::~PrintObserver()
{
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
