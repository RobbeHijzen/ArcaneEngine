#pragma once

#include "SceneInfo.h"
#include "Scene.h"

class HighscoreLevel : public AE::SceneInfo
{
public:

	virtual void Load(AE::Scene& scene) override;

private:
	void DrawBackground(AE::Scene& scene);

	void AddHighscoresText(AE::Scene& scene);
	void AddScoreInfos(AE::Scene& scene);
	void AddButtons(AE::Scene& scene);
};
