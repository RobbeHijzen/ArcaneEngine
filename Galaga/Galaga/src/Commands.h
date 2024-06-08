#pragma once

#include "ArcaneEngine.h"

#include "Time.h"
#include "HealthComponent.h"
#include "ShootComponent.h"
#include "ButtonBoxComponent.h"
#include "Movementcomponent.h"
#include "FSMComponent.h"
#include "StatesEnemyBoss.h"

#include "SceneManager.h"


class MoveCommand : public AE::GameObjectCommand
{
public:
	MoveCommand(AE::GameObject* gameObject, glm::vec2 direction, float movementSpeed, MovementComponent* moveComp)
		: GameObjectCommand(gameObject)
		, m_MovementSpeed{ movementSpeed }
		, m_MoveComp{moveComp}
	{
		assert(std::abs(direction.x) > 0 || std::abs(direction.y) > 0);
		m_Direction = glm::normalize(direction);
	}

	virtual void Execute() override
	{
		glm::vec2 addedPosition{ m_Direction * m_MovementSpeed * AE::Time::GetInstance().GetDeltaTime() };
		m_MoveComp->Move(addedPosition);
	}

private:
	glm::vec2 m_Direction;
	float m_MovementSpeed;
	MovementComponent* m_MoveComp{};
};


class ShootCommand : public AE::GameObjectCommand
{
public:

	ShootCommand(AE::GameObject* gameObject)
		: GameObjectCommand(gameObject)
	{
		m_ShootComp = gameObject->GetComponent<ShootComponent>();
	}

	virtual void Execute() override
	{
		if (m_ShootComp)
		{
			m_ShootComp->FireBullet();
		}
	}

private:
	std::shared_ptr<ShootComponent> m_ShootComp{};

};

class LoadCommand : public AE::GameObjectCommand
{
public:

	LoadCommand(AE::GameObject* gameObject, std::string newSceneName)
		: GameObjectCommand(gameObject)
		, m_SceneName{newSceneName}
	{}

	virtual void Execute() override
	{
		AE::SceneManager::GetInstance().SetScene(m_SceneName);
	}
private:
	std::string m_SceneName{};
};

class ButtonPressCommand : public AE::GameObjectCommand
{
public:

	ButtonPressCommand(AE::GameObject* gameObject)
		: GameObjectCommand(gameObject)
	{}

	virtual void Execute() override
	{
		if (auto buttonBoxComp = GetGameObject()->GetComponent<ButtonBoxComponent>())
		{
			buttonBoxComp->PressCurrentButton();
		}
	}
};
class ButtonMoveSelectionCommand : public AE::GameObjectCommand
{
public:

	ButtonMoveSelectionCommand(AE::GameObject* gameObject, glm::i32vec2 moveDir)
		: GameObjectCommand(gameObject)
		, m_MoveDir{moveDir}
	{}

	virtual void Execute() override
	{
		if (auto buttonBoxComp = GetGameObject()->GetComponent<ButtonBoxComponent>())
		{
			buttonBoxComp->MoveSelection(m_MoveDir);
		}
	}

private:
	glm::i32vec2 m_MoveDir{};
	
};

class BossGalagaDiveCommand : public AE::GameObjectCommand
{
public:

	BossGalagaDiveCommand(AE::GameObject* gameObject)
		: GameObjectCommand(gameObject)
	{
	}

	virtual void Execute() override
	{
		if (auto fsm = GetGameObject()->GetComponent<FSMComponent>().get())
		{
			if (auto idleState = dynamic_cast<StatesEnemyBoss::Idle*>(fsm->GetCurrentState()))
			{
				idleState->ChangeToBombingRun();
			}
		}
	}

	void SetGameObject(AE::GameObject* go) { ChangeGameObject(go); }

private:
};
class BossGalagaBeamCommand : public AE::GameObjectCommand
{
public:

	BossGalagaBeamCommand(AE::GameObject* gameObject)
		: GameObjectCommand(gameObject)
	{
	}

	virtual void Execute() override
	{
		if (auto fsm = GetGameObject()->GetComponent<FSMComponent>().get())
		{
			if (auto idleState = dynamic_cast<StatesEnemyBoss::Idle*>(fsm->GetCurrentState()))
			{
				idleState->ChangeToTractorBeam();
			}
		}
	}

	void SetGameObject(AE::GameObject* go) { ChangeGameObject(go); }

private:
};
class BossGalagaShootCommand : public AE::GameObjectCommand
{
public:

	BossGalagaShootCommand(AE::GameObject* gameObject)
		: GameObjectCommand(gameObject)
	{
	}

	virtual void Execute() override
	{
		if (auto shootComp = GetGameObject()->GetComponent<ShootComponent>().get())
		{
			shootComp->FireBullet();
		}
	}

	void SetGameObject(AE::GameObject* go) { ChangeGameObject(go); }

private:
};

