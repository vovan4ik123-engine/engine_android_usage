#include "PlayGUILayer.h"

namespace
{
    void showFrameTime()
    {
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::Begin("FrameTime", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                           ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);


        ImGui::Text("Frame time = %.3u, FPS = %.3u", Beryll::GameLoop::getFrameTime(), Beryll::GameLoop::getFPS());
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(0.0f, 60.0f));
        ImGui::Begin("Size", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                           ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);


        ImGui::Text("GUI DisplaySize = %.1f,  %.1f", Beryll::MainImGUI::getInstance()->getGUIWidth(), Beryll::MainImGUI::getInstance()->getGUIHeight());

        ImGui::End();
    }
}

PlayGUILayer::PlayGUILayer()
{
    m_ID = Beryll::LayerID::PLAY_GUI;

    guiDemo = std::make_shared<Beryll::DemoImGUI>();
    buttonUp = std::make_shared<Beryll::Button>("1", 0, 90, 29, 9);

    buttonMiddle = std::make_shared<Beryll::Button>("STOP", 30, 90, 29, 9, true);
    buttonMiddle->setColor(0.4f, 0.0f, 0.4f, 1.0f);
    buttonMiddle->setClickedColor(0.6f, 0.0f, 0.6f, 1.0f);
    buttonMiddle->setFontColor(1.0f, 0.0f, 0.0f, 1.0f);
    buttonMiddle->setSoundWAV("sounds/gren2.wav");

    buttonDown = std::make_shared<Beryll::Button>("2", 60, 90, 29, 9);

    buttonResetCube = std::make_shared<Beryll::Button>("Reset", 0, 80, 29, 9);

    buttonMove = std::make_shared<Beryll::Button>("M", 30, 80, 29, 9, true);
    buttonPause = std::make_shared<Beryll::Button>("Pause", 60, 80, 29, 9);

    checkBox1 = std::make_shared<Beryll::CheckBox>("Test box 1", 10, 10);
    checkBox1->setSoundWAV("sounds/gren2.wav");

    checkBox2 = std::make_shared<Beryll::CheckBox>("Box 2", 10, 40);

    text1 = std::make_shared<Beryll::Text>("Text test1", 30, 50);

    text2 = std::make_shared<Beryll::Text>("Text test 2 \nnew line here", 30, 60, true);
    text2->setBackgroundColor(0.0f, 0.0f, 0.4f, 0.7f);
    text2->setFontColor(0.0f, 1.0f, 0.0f, 1.0f);

    drawFrameTime = std::make_shared<Beryll::DrawAnyFunction>(showFrameTime);

    // add gameObjects to MyLayer
    //m_gameObjects.push_back(guiDemo);
    m_gameObjects.push_back(buttonUp);
    m_gameObjects.push_back(buttonMiddle);
    m_gameObjects.push_back(buttonDown);
    m_gameObjects.push_back(buttonResetCube);
    m_gameObjects.push_back(buttonMove);
    m_gameObjects.push_back(buttonPause);
    //m_gameObjects.push_back(checkBox1);
    //m_gameObjects.push_back(checkBox2);
    //m_gameObjects.push_back(text1);
    //m_gameObjects.push_back(text2);
    m_gameObjects.push_back(drawFrameTime);

    // create pause state + layers
    pauseGUILayer = std::make_shared<PauseGUILayer>();

    pauseState = std::make_shared<Beryll::GameState>();
    pauseState->ID = Beryll::GameStateID::PAUSE;
    pauseState->layerStack.pushOverlay(pauseGUILayer);

    Beryll::SoundsManager::loadBackgroundMP3("sounds/MP3/JOY.mp3");
    Beryll::SoundsManager::loadBackgroundMP3("sounds/MP3/Fancy.mp3");
}
PlayGUILayer::~PlayGUILayer()
{

}

void PlayGUILayer::updateBeforePhysics()
{
    for(const std::shared_ptr<Beryll::GameObject>& go : m_gameObjects)
    {
        if(go->getIsEnabled())
        {
            go->updateBeforePhysics();
        }
    }

    if(buttonPause->getIsPressed())
    {
        Beryll::Physics::disableSimulation();
        Beryll::GameStateMachine::pushState(pauseState);
    }
}

void PlayGUILayer::updateAfterPhysics()
{
    // dont need update GUI objects after physics symulation
}

void PlayGUILayer::draw()
{
    for(const std::shared_ptr<Beryll::GameObject>& go : m_gameObjects)
    {
        if(go->getIsEnabled())
        {
            go->draw();
        }
    }
}

void PlayGUILayer::playSound()
{
    for(const std::shared_ptr<Beryll::GameObject>& go : m_gameObjects)
    {
        if(go->getIsEnabled())
        {
            go->playSound();
        }
    }

    if(buttonUp->getIsPressed() && !Beryll::SoundsManager::getIsBackgroundMP3Started())
    {
        Beryll::SoundsManager::startBackgroundMP3("sounds/MP3/JOY.mp3");
    }
    else if(buttonUp->getIsPressed() && Beryll::SoundsManager::getIsBackgroundMP3Playing())
    {
        Beryll::SoundsManager::pauseBackgroundMP3();
    }
    else if(buttonUp->getIsPressed() && Beryll::SoundsManager::getIsBackgroundMP3Paused())
    {
        Beryll::SoundsManager::resumeBackgroundMP3();
    }

    if(buttonDown->getIsPressed() && !Beryll::SoundsManager::getIsBackgroundMP3Started())
    {
        Beryll::SoundsManager::startBackgroundMP3("sounds/MP3/Fancy.mp3");
    }
    else if(buttonDown->getIsPressed() && Beryll::SoundsManager::getIsBackgroundMP3Playing())
    {
        Beryll::SoundsManager::pauseBackgroundMP3();
    }
    else if(buttonDown->getIsPressed() && Beryll::SoundsManager::getIsBackgroundMP3Paused())
    {
        Beryll::SoundsManager::resumeBackgroundMP3();
    }

    if(buttonMiddle->getIsPressed())
    {
        Beryll::SoundsManager::stopBackgroundMP3();
    }
}