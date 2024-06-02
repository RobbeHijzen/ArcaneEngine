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

void ButtonBoxComponent::AddHorizontalButton(ButtonComponent* button)
{
	if(m_Buttons.size() == 0) m_Buttons.resize(1);
	m_Buttons[m_Buttons.size() - 1].emplace_back(button);
}

void ButtonBoxComponent::AddVerticalButton(ButtonComponent* button)
{
	m_Buttons.emplace_back(std::vector<ButtonComponent*>{ button });
}

void ButtonBoxComponent::MoveImageToSelection()
{
	glm::vec3 newPos{ m_Buttons[m_CurrentlySelectedRow][m_CurrentlySelectedCol]->GetWorldTransform().GetPosition() };
	m_SelectionImageComp->SetLocalPosition(newPos.x + m_SelectionOffset.x, newPos.y + m_SelectionOffset.y);
}
