#include "VersusLevel.h"

#include "InputManager.h"
#include "ServiceLocator.h"
#include "Commands.h"

void VersusLevel::Load(AE::Scene& )
{

	// Mute command
	AE::InputManager::GetInstance().BindActionKB(SDL_SCANCODE_M, AE::InputType::IsPressed, std::move(std::make_unique<AE::LambdaCommand>([&]() {AE::ServiceLocator::GetAudio()->ToggleMute(); })));
}
