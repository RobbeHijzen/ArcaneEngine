#include "ScoreComponent.h"

#include "GameObject.h"
#include "ObserverEvents.h"

void ScoreComponent::IncreaseScore(int scoreAmount)
{
	m_Score += scoreAmount;

	GetOwner()->Notify(ObserverPattern::Event::ScoreChanged);

}
