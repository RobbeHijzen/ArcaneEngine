#pragma once

#include "SceneInfo.h"
#include "Scene.h"
#include "GalagaGameInstance.h"
#include "Transform.h"
#include "ButtonComponent.h"
#include "TextComponent.h"

class SaveLevel : public AE::SceneInfo
{
public:

	virtual void Load(AE::Scene& scene) override;

private:
	// Greyed out background
	void DrawBackground(AE::Scene& scene);

	void AddHighscoresText(AE::Scene& scene);
	void AddScoreInfos(AE::Scene& scene, std::vector<ScoreInfo>& highscores);

	// keyboard and name stuff
	void AddCentralNameText(AE::Scene& scene);
	void AddButtons(AE::Scene& scene, const std::vector<ScoreInfo>& highscores);
	ButtonComponent* MakeButton(AE::Scene& scene, glm::vec2 pos, std::string addition, AE::Rect sourceRect, std::string texture);

	void WriteHighscores(const std::vector<ScoreInfo>& highscores, std::string newName);

	void AppendName(std::string addition)
	{
		m_PlayerName.append(addition);
		m_PlayerTextComp->SetText(m_PlayerName);
	}

	std::string m_PlayerName{};
	TextComponent* m_PlayerTextComp{};
};

