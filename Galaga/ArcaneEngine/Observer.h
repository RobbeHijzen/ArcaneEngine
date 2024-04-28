#pragma once

#include <iostream>
#include <vector>
#include "ObserverEvents.h"


class GameObject;

namespace ObserverPattern
{
	class Observer
	{
	public:

		virtual ~Observer() = default;
		virtual void OnNotify(Event event, GameObject* gameObject) = 0;

	};

}

