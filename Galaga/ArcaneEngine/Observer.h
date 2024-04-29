#pragma once

#include <iostream>
#include <vector>
#include "ObserverEvents.h"

namespace AE
{
	class GameObject;

	class Observer
	{
	public:

		virtual ~Observer() = default;
		virtual void OnNotify(Event event, GameObject* gameObject) = 0;

	};

}

