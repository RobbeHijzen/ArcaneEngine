#include "ButtonBoxComponent.h"

void ButtonBoxComponent::GameStart()
{
	MoveImageToSelection();
}

void ButtonBoxComponent::PressCurrentButton()
{
	m_Buttons[m_CurrentlySelectedRow][m_CurrentlySelectedCol]->Press();
}

void ButtonBoxComponent::MoveSelection(glm::i32vec2 dir)
{
	if (dir.x == 0)
	{
		int rowSize{ int(m_Buttons.size()) };
		if (rowSize > 1)
		{
			m_Buttons[m_CurrentlySelectedRow][m_CurrentlySelectedCol]->OnDeselect();

			m_CurrentlySelectedRow = (m_CurrentlySelectedRow + dir.y) % rowSize;
			if (m_CurrentlySelectedRow < 0) m_CurrentlySelectedRow += rowSize;

			m_CurrentlySelectedCol = std::min(m_CurrentlySelectedCol, int(m_Buttons[m_CurrentlySelectedRow].size() - 1));
			
			m_Buttons[m_CurrentlySelectedRow][m_CurrentlySelectedCol]->OnSelect();
			MoveImageToSelection();
		}
	}
	else if (dir.y == 0)
	{
		int colSize{ int(m_Buttons[m_CurrentlySelectedRow].size()) };
		if (colSize > 1)
		{
			m_Buttons[m_CurrentlySelectedRow][m_CurrentlySelectedCol]->OnDeselect();

			m_CurrentlySelectedCol = (m_CurrentlySelectedCol + dir.x) % colSize;
			if (m_CurrentlySelectedCol < 0) m_CurrentlySelectedCol += colSize;

			m_Buttons[m_CurrentlySelectedRow][m_CurrentlySelectedCol]->OnSelect();
			MoveImageToSelection();
		}
	}
}

void ButtonBoxComponent::AddHorizontalButton(ButtonComponent* button, glm::vec2 buttonSize)
{
	if (m_Buttons.size() == 0) { m_Buttons.resize(1); m_ButtonFollowSizeInfo.resize(1); }
	m_Buttons[m_Buttons.size() - 1].emplace_back(button);
	m_ButtonFollowSizeInfo[m_Buttons.size() - 1].emplace_back(std::pair<bool, glm::vec2>{true, buttonSize});
}

void ButtonBoxComponent::AddHorizontalButton(ButtonComponent* button)
{
	if (m_Buttons.size() == 0) { m_Buttons.resize(1); m_ButtonFollowSizeInfo.resize(1); }
	m_Buttons[m_Buttons.size() - 1].emplace_back(button);
	m_ButtonFollowSizeInfo[m_Buttons.size() - 1].emplace_back(std::pair<bool, glm::vec2>{false, {}});
}

void ButtonBoxComponent::AddVerticalButton(ButtonComponent* button, glm::vec2 buttonSize)
{
	m_Buttons.emplace_back(std::vector<ButtonComponent*>{button});
	m_ButtonFollowSizeInfo.emplace_back(std::vector<std::pair<bool, glm::vec2>>{{true, buttonSize}});
}

void ButtonBoxComponent::AddVerticalButton(ButtonComponent* button)
{
	m_Buttons.emplace_back(std::vector<ButtonComponent*>{button});
	m_ButtonFollowSizeInfo.emplace_back(std::vector<std::pair<bool, glm::vec2>>{{false, {}}});
}

void ButtonBoxComponent::MoveImageToSelection()
{
	if (m_Buttons.size() == 0) return;
	glm::vec3 newPos{ m_Buttons[m_CurrentlySelectedRow][m_CurrentlySelectedCol]->GetWorldTransform().GetPosition() };
	m_SelectionImageComp->SetLocalPosition(newPos.x + m_SelectionOffset.x, newPos.y + m_SelectionOffset.y);

	if (m_ButtonFollowSizeInfo[m_CurrentlySelectedRow][m_CurrentlySelectedCol].first)
	{
		glm::vec2 size{ m_ButtonFollowSizeInfo[m_CurrentlySelectedRow][m_CurrentlySelectedCol].second };
		AE::Rect newDestRect{ 0.f,0.f, size.x, size.y };
		m_SelectionImageComp->SetDestRect(newDestRect);
	}
}
