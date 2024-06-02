#pragma once

#include "SceneInfo.h"
#include "Scene.h"

class DeathScreenLevel : public AE::SceneInfo
{
public:

	virtual void Load(AE::Scene& scene) override;

private:

	void DrawBackground(AE::Scene& scene);
	void AddResults(AE::Scene& scene);
	void AddContinueButton(AE::Scene& scene);
};

