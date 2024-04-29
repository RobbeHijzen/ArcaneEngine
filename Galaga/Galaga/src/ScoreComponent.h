#pragma once

#include "BaseComponent.h"


class ScoreComponent : public BaseComponent
{
public:

	ScoreComponent(GameObject* pParent)
		: BaseComponent(pParent)
	{
		//pParent->
	}


	void IncreaseScore(int scoreAmount);
	int GetScore() const { return m_Score; }

private:

	int m_Score{ 0 };

};

