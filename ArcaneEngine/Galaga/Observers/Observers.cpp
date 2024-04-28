#include "Observers.h"

#include "GameObject.h"

#include "HealthComponent.h"
#include "ScoreComponent.h"

using namespace ObserverPattern;


void PrintObserver::OnNotify(Event event, GameObject*)
{
	switch (event)
	{
		case Event::PrintTest:
		{
			std::cout << "Test Printed\n";
		}
	}
}

void ObserverPattern::HealthDisplayObserver::OnNotify(Event event, GameObject* gameObject)
{
	switch (event)
	{
	case Event::ObjectDied:
	{
		int newHealth{ gameObject->GetComponent<HealthComponent>()->GetHealth() };

		std::string newText{"#Lives: " + std::to_string(newHealth)};
		m_pTextComponent->SetText(newText);

	}
	}
}

void ObserverPattern::ScoreDisplayObserver::OnNotify(Event event, GameObject* gameObject)
{
	switch (event)
	{
	case Event::ScoreChanged:
	{
		int newScore{ gameObject->GetComponent<ScoreComponent>()->GetScore() };

		std::string newText{ "#Score: " + std::to_string(newScore) };
		m_pTextComponent->SetText(newText);
	}
	}
}
