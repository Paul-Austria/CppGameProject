// GameProject.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include "GameProject.h"

#include <Engine/Engine.hpp>
#include <spdlog/spdlog.h>

using namespace std;

int main()
{

	GameEngine::Engine::GetInstance()->Init(1920/2,1080/2,"Engine");
//	GameEngine::Engine::GetInstance()->SetCurrentScene(scene);
	GameEngine::Engine::GetInstance()->Loop();
	return 0;
}
