#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "ArcaneEngine.h"
#include "GalagaGameInstance.h"

#include "CoopLevel.h"
#include "DeathScreenLevel.h"
#include "LeaderboardLevel.h"
#include "SoloLevel.h"
#include "StartScreenLevel.h"
#include "VersusLevel.h"
#include "SaveLevel.h"
#include "HighscoreLevel.h"

using namespace AE;

void SetupScenes()
{
	SceneManager::GetInstance().SetGameInstance(std::move(std::make_unique<GalagaGameInstance>()));


	SceneManager::GetInstance().CreateScene(std::move(std::make_unique<StartScreenLevel>()), "StartScreen");

	SceneManager::GetInstance().CreateScene(std::move(std::make_unique<SoloLevel>()), "SoloLevel");
	SceneManager::GetInstance().CreateScene(std::move(std::make_unique<CoopLevel>()), "CoopLevel");
	SceneManager::GetInstance().CreateScene(std::move(std::make_unique<VersusLevel>()), "VersusLevel");

	SceneManager::GetInstance().CreateScene(std::move(std::make_unique<DeathScreenLevel>()), "DeathScreen");
	SceneManager::GetInstance().CreateScene(std::move(std::make_unique<LeaderboardLevel>()), "LeaderboardScreen");
	
	SceneManager::GetInstance().CreateScene(std::move(std::make_unique<HighscoreLevel>()), "HighscoreScreen");
	SceneManager::GetInstance().CreateScene(std::move(std::make_unique<SaveLevel>()), "SaveScreen");
}

int main(int, char* [])
{
	ArcaneEngine engine("../Resources/");
	engine.Run(SetupScenes);

	return 0;
}


