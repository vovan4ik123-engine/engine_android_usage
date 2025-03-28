#include "EngineHeaders.h"

// app files
#include "PlayGUILayer.h"
#include "Play3DSceneLayer.h"

int main(int argc, char* argv[])
{
    BR_INFO("%s", "main() started");
    Beryll::GameLoop::create(); // must be called first
    //Beryll::GameLoop::setMaxFPS(7);

    // GUI elements properties
    Beryll::MainImGUI::getInstance()->setButtonsFont("fonts/arial-italic.ttf", 5);
    Beryll::MainImGUI::getInstance()->setCheckBoxesFont("fonts/creamy.ttf", 3);
    Beryll::MainImGUI::getInstance()->setSlidersFont("fonts/creamy.ttf", 3);
    Beryll::MainImGUI::getInstance()->setTextsFont("fonts/proggy.ttf", 2);

    // camera properties
    Beryll::Camera::setPerspectiveNearClipPlane(3.0f);
    Beryll::Camera::setPerspectiveFarClipPlane(300.0f);
    Beryll::Camera::setObjectsViewDistance(200.0f);

    // Simulation precision (1-20 range), also increase CPU usage
    // Increase it if your ball penetrates wall but you want collision
    Beryll::Physics::setResolution(4);

    Beryll::LoadingScreen::setTextures({"loadingScreenImages/lowPolyTerrain1.jpg",
                                                   "loadingScreenImages/lowPolyTerrain2.jpg"});

    // Create own class MyLayer and inherit from Beryll::Layer provided by engine
    std::shared_ptr<PlayGUILayer> playGUILayer = std::make_shared<PlayGUILayer>();
    // scene layer will have access to elements from GUI layer
    std::shared_ptr<Play3DSceneLayer> playSceneLayer = std::make_shared<Play3DSceneLayer>(playGUILayer);

    // create object of GameState provided by engine + set ID
    std::shared_ptr<Beryll::GameState> playState = std::make_shared<Beryll::GameState>();
    playState->ID = Beryll::GameStateID::PLAY;
    // Push MyLayer to GameState->layerStack
    playState->layerStack.pushLayer(playSceneLayer);
    playState->layerStack.pushOverlay(playGUILayer); // pushOverlay for GUI

    // Push gameState to GameStateMachine
    Beryll::GameStateMachine::pushState(playState);

    Beryll::GameLoop::run();

    return 0;
}
