#pragma once

#include <iostream>
#include <vector>
#include "ObserverEvents.h"

#include "TextComponent.h"

class GameObject;

namespace ObserverPattern
{
	class Observer
	{
	public:

		virtual ~Observer() = default;
		virtual void OnNotify(Event event, GameObject* gameObject) = 0;

	};


	class PrintObserver : public Observer
	{
	public:

		virtual void OnNotify(Event event, GameObject* gameObject) override;
	};

	class HealthDisplayObserver : public Observer
	{
	public:

		HealthDisplayObserver(TextComponent* textComp) 
			: m_pTextComponent{textComp}
		{}

		virtual void OnNotify(Event event, GameObject* gameObject) override;

	private:

		TextComponent* m_pTextComponent{};

	};
	class ScoreDisplayObserver : public Observer
	{
	public:

		ScoreDisplayObserver(TextComponent* textComp)
			: m_pTextComponent{ textComp } 
		{}

		virtual void OnNotify(Event event, GameObject* gameObject) override;

	private:

		TextComponent* m_pTextComponent{};

	};

}

