#pragma once
#include "GameInstance.h"
#include "GameObject.h"
#include "ObserverEvents.h"

#include <string>


class GalagaGameInstance : public AE::GameInstance
{
public:
	GalagaGameInstance() {}
	virtual ~GalagaGameInstance() {}

	int GetScore() const { return m_Score; }
	int GetShotsFired() const { return m_ShotsFired; }
	int GetShotsHit() const { return m_ShotsHit; }

	void IncreaseScore(int addedScore, AE::GameObject* gameObject) 
	{ 
		m_Score += addedScore; 
		gameObject->NotifyAll(AE::Event::ScoreChanged); 
	}
	void IncrementShotsFired() { ++m_ShotsFired; }
	void IncrementShotsHit() { ++m_ShotsHit; }

	void ResetStats() 
	{ 
		m_Score = 0; 
		m_ShotsFired = 0; 
		m_ShotsHit = 0; 
	}

private:

	int m_Score{};

	int m_ShotsFired{};
	int m_ShotsHit{};

};

struct ScoreInfo
{
	std::string name{};
	int score{};
};

