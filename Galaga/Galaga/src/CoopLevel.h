#pragma once

#include "SceneInfo.h"
#include "Scene.h"

class CoopLevel : public AE::SceneInfo
{
public:

	virtual void Load(AE::Scene& scene) override;

private:

	void AddBackgroundImage(AE::Scene& scene);
	AE::GameObject* AddGalaga1(AE::Scene& scene);
	AE::GameObject* AddGalaga2(AE::Scene& scene);
	void AddSpawnerManager(AE::Scene& scene, std::vector<AE::GameObject*> galagas);
	void CreateSurroundingHitBoxes(AE::Scene& scene);
};
