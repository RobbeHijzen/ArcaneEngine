#pragma once
#include "FSMState.h"
#include "EnemySpawningStructs.h"

#include <list>

namespace StatesEnemy
{
	//------------------
	// General states used by all enemies
	//------------------
	class Moving : public AE::FSMState
	{
	public:
		Moving(std::unique_ptr<AE::FSMState> nextState, std::list<EnemySeekInfo> seekInfo)
			: m_NextState{std::move(nextState)}
			, m_SeekInfo{ seekInfo } {}

		virtual void OnEnter(AE::GameObject*) override;
		virtual void OnExit(AE::GameObject*) override {};
		virtual std::unique_ptr<AE::FSMState> Update(AE::GameObject* gameObject) override;

	private:
		std::unique_ptr<AE::FSMState> m_NextState{};

		std::list<EnemySeekInfo> m_SeekInfo{};
		EnemySeekInfo m_CurrentSeekInfo{};

		const float m_CircleRadius{45.f};
		const int m_CircleSeekAmount{16};

		float m_MoveSpeed{ 90.f };
		void MoveAccordingToSeekInfo(EnemySeekInfo seekInfo, AE::GameObject* gameObject);
	};

}

