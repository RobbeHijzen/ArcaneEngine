#pragma once

#include "SceneInfo.h"
#include "Scene.h"

class Level_02 : public AE::SceneInfo
{
public:

	virtual void Load(AE::Scene& scene) override;

private:

	void AddBackgroundImage(AE::Scene& scene);
	void AddGalaga(AE::Scene& scene);
	void AddBossEnemy(AE::Scene& scene);
	void AddControlsExplainers(AE::Scene& scene);
};

