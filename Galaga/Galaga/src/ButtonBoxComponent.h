#pragma once

#include "BaseComponent.h"
#include "GameObject.h"
#include "ButtonComponent.h"
#include "ImageComponent.h"

class ButtonBoxComponent : public AE::BaseComponent
{
public:
	ButtonBoxComponent(AE::GameObject* pParent, ImageComponent* selectionImageComp, glm::vec2 selectionOffset = {})
		: BaseComponent(pParent)
		, m_SelectionImageComp{selectionImageComp}
		, m_SelectionOffset{selectionOffset}
	{
	}

	void GameStart() override;

	void PressCurrentButton();
	void MoveSelection(glm::i32vec2 dir);

	void AddHorizontalButton(ButtonComponent* button, glm::vec2 buttonSize);
	void AddHorizontalButton(ButtonComponent* button);
	void AddVerticalButton(ButtonComponent* button, glm::vec2 buttonSize);
	void AddVerticalButton(ButtonComponent* button);

private:

	std::vector<std::vector<ButtonComponent*>> m_Buttons{};
	std::vector<std::vector<std::pair<bool, glm::vec2>>> m_ButtonFollowSizeInfo{};

	int m_CurrentlySelectedRow{};
	int m_CurrentlySelectedCol{};

	ImageComponent* m_SelectionImageComp{};
	glm::vec2 m_SelectionOffset{};

	void MoveImageToSelection();
};



