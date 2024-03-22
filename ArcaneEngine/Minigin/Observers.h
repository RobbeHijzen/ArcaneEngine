#pragma once

#include <iostream>
#include <vector>
#include "ObserverEvents.h"

class GameObject;
class Subject;

namespace ObserverPattern
{
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(Event event, GameObject* gameObject) = 0;

	};


	class PrintObserver : public Observer
	{
	public:
		virtual ~PrintObserver();
		virtual void Notify(Event event, GameObject* gameObject) override;
	};
}

