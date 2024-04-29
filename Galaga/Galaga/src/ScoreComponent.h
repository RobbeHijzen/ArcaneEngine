#pragma once

#include "BaseComponent.h"


class ScoreComponent : public AE::BaseComponent
{
public:

	ScoreComponent(AE::GameObject* pParent)
		: BaseComponent(pParent)
	{
	}


	void IncreaseScore(int scoreAmount);
	int GetScore() const { return m_Score; }

private:

	int m_Score{ 0 };

};

