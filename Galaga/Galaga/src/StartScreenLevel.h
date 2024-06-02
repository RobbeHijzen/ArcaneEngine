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
	void AddButtons(AE::Scene& scene);
};

