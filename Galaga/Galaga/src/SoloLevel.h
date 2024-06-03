#pragma once

#include "SceneInfo.h"
#include "Scene.h"

class SoloLevel : public AE::SceneInfo
{
public:

	virtual void Load(AE::Scene& scene) override;

private:

	void AddBackgroundImage(AE::Scene& scene);
	AE::GameObject* AddGalaga(AE::Scene& scene);
	void AddSpawnerManager(AE::Scene& scene, AE::GameObject* galaga);
};

