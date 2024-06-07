#pragma once
#include "SceneInfo.h"
#include "Scene.h"

class StartScreenLevel : public AE::SceneInfo
{
public:

	virtual void Load(AE::Scene& scene) override;

private:

	void AddBackgroundImage(AE::Scene& scene);
	void AddLogoImage(AE::Scene& scene);
	void AddButtons(AE::Scene& scene, AE::GameObject* birdControls);

	void AddRegularControls(AE::Scene& scene);
	AE::GameObject* AddBirdControls(AE::Scene& scene);
};

