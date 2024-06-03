#pragma once
#include <string>
#include <memory>
#include "GameObject.h"
#include "Transform.h"
#include "ImageComponent.h"

class MovementImageComponent final : public AE::BaseComponent
{
public:

	MovementImageComponent(AE::GameObject* parentGameObject, const std::string& fileName
						, glm::vec2 imageOffset, int numMoveDirections, glm::vec2 startDir);

	void SetSourceRect(AE::Rect sourceRect);
	void SetSourcePos(glm::vec2 pos);
	void SetDestRect(AE::Rect rect);

	void LateUpdate() override;

private:

	ImageComponent* m_ImageComp{};

	AE::Rect m_OriginalSourceRect{};
	glm::vec2 m_ImageOffset{};

	int m_NumDirections{};
	float m_DegreeIncrement{360.f / m_NumDirections };
	float m_StartingDegree{};

	glm::vec2 m_LastFramePos{};

	float NegativeDegToPosOnlyDeg(float degrees);
};




