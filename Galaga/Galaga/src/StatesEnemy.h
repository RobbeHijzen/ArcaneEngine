#pragma once
#include "EngineStates.h"
#include "EnemySpawningStructs.h"

#include <queue>

namespace StatesEnemy
{
	//------------------
	// General states used by all enemies
	//------------------
	class Spawning : public AE::FSMState
	{
	public:
		Spawning(std::unique_ptr<AE::FSMState> nextState, std::queue<EnemySeekInfo> seekInfo)
			: m_NextState{std::move(nextState)}
			, m_SeekInfo{ seekInfo } {}

		virtual void OnEnter(AE::GameObject*) override;
		virtual void OnExit(AE::GameObject*) override {};
		virtual std::unique_ptr<AE::FSMState> Update(AE::GameObject* gameObject) override;

	private:
		std::unique_ptr<AE::FSMState> m_NextState{};

		std::queue<EnemySeekInfo> m_SeekInfo{};
		EnemySeekInfo m_CurrentSeekInfo{};


		float m_MoveSpeed{ 130.f };
		void MoveAccordingToSeekInfo(EnemySeekInfo seekInfo, AE::GameObject* gameObject);
	};

}

