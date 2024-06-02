#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "ArcaneEngine.h"

#include "StartScreenLevel.h"
#include "SoloLevel.h"

using namespace AE;

void SetupScenes()
{
	SceneManager::GetInstance().CreateScene(std::move(std::make_unique<StartScreenLevel>()), "StartScreen");
	SceneManager::GetInstance().CreateScene(std::move(std::make_unique<SoloLevel>()), "SoloLevel");

}

int main(int, char* [])
{
	ArcaneEngine engine("../Resources/");
	engine.Run(SetupScenes);

	return 0;
}


