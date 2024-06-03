#pragma once
#include "FSMState.h"

#include "Transform.h"
#include "GameObject.h"

namespace AE
{
	namespace States
	{
		class Seek : public FSMState
		{
		public:
			Seek(std::unique_ptr<AE::FSMState> nextState, glm::vec2 seekPos, float moveSpeed = 130.f)
				: m_NextState{std::move(nextState)}
				, m_SeekPos{seekPos}
				, m_MoveSpeed{moveSpeed}
			{}

			virtual void OnEnter(AE::GameObject* gameObject) override;
			virtual void OnExit(AE::GameObject*) override {};
			virtual std::unique_ptr<AE::FSMState> Update(AE::GameObject* gameObject) override;

		private:
			float m_MoveSpeed{ 130.f };

			glm::vec2 m_SeekDir{};
			glm::vec2 m_SeekPos{};

			std::unique_ptr<AE::FSMState> m_NextState{};

		};
	}

	namespace StateHelpers
	{
		bool HasReached(glm::vec2 seekPos, AE::GameObject* gameObject, float acceptanceRadius = 1.f);
		glm::vec2 GetSeekDirection(glm::vec2 seekPos, AE::GameObject* gameObject);

	}
}


