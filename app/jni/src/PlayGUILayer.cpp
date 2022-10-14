#include "PlayGUILayer.h"

namespace
{
    void showFrameTime()
    {
        static uint32_t frameTime = 0;
        static uint32_t drawTime = 0;
        static uint32_t calcTime = 0;
        static uint32_t FPS = 0;
        static uint32_t lastUpdate = 0;

        if(Beryll::TimeStep::getMillisecFromStart() > lastUpdate + 200)
        {
            frameTime = Beryll::GameLoop::getFrameTime();
            FPS = Beryll::GameLoop::getFPS();
            drawTime = Beryll::GameLoop::drawTime;
            calcTime = Beryll::GameLoop::calcTime;

            lastUpdate = Beryll::TimeStep::getMillisecFromStart();
        }

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.0f, 0.0f, 0.0f, 1.0f });

        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::Begin("FrameTime", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                           ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);


        ImGui::Text("Frame time:%.3u, FPS:%.3u", frameTime, FPS);
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(0.0f, 32.0f));
        ImGui::Begin("DrawTime", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                           ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);


        ImGui::Text("Draw  time:%.3u, Calc time:%.3u", drawTime, calcTime);
        ImGui::End();

        ImGui::PopStyleColor(1);
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

    buttonJump = std::make_shared<Beryll::Button>("Jump", 0, 80, 29, 9);

    buttonMove = std::make_shared<Beryll::Button>("Move", 30, 80, 29, 9, true);
    buttonPause = std::make_shared<Beryll::Button>("Pause", 60, 80, 29, 9);

    checkBox1 = std::make_shared<Beryll::CheckBox>("Test box 1", 10, 10);
    checkBox1->setSoundWAV("sounds/gren2.wav");

    checkBox2 = std::make_shared<Beryll::CheckBox>("Box 2", 10, 40);

    text1 = std::make_shared<Beryll::Text>("Text test1", 30, 50);

    text2 = std::make_shared<Beryll::Text>("Text test 2 \nnew line here", 30, 60, true);
    text2->setBackgroundColor(0.0f, 0.0f, 0.4f, 0.7f);
    text2->setFontColor(0.0f, 1.0f, 0.0f, 1.0f);

    sliderCamera = std::make_shared<Beryll::Slider>("Camera distance", 0, 8, 50, 3, true);
    sliderCamera->setFontColor(0.0f, 0.0f, 0.0f, 1.0f);
    sliderCamera->setTextBackgroundColor(1.0f, 1.0f, 1.0f, 0.3f);
    sliderCamera->setDragAreaColor(1.0f, 0.0f, 0.0f, 1.0f);
    sliderCamera->setSliderGrabColor(0.0f, 0.0f, 0.0f, 1.0f);

    sliderSunPower = std::make_shared<Beryll::Slider>("Sun power", 0, 13, 50, 3, true);
    sliderSunPower->setFontColor(0.0f, 0.0f, 0.0f, 1.0f);
    sliderSunPower->setTextBackgroundColor(1.0f, 1.0f, 1.0f, 0.3f);
    sliderSunPower->setDragAreaColor(1.0f, 0.0f, 0.0f, 1.0f);
    sliderSunPower->setSliderGrabColor(0.0f, 0.0f, 0.0f, 1.0f);

    drawFrameTime = std::make_shared<Beryll::DrawAnyFunction>(showFrameTime);

    // add GUIObjects to MyLayer
    //m_guiObjects.push_back(guiDemo);
    m_guiObjects.push_back(buttonUp);
    m_guiObjects.push_back(buttonMiddle);
    m_guiObjects.push_back(buttonDown);
    m_guiObjects.push_back(buttonJump);
    m_guiObjects.push_back(buttonMove);
    m_guiObjects.push_back(buttonPause);
    m_guiObjects.push_back(sliderCamera);
    m_guiObjects.push_back(sliderSunPower);
    //m_guiObjects.push_back(checkBox1);
    //m_guiObjects.push_back(checkBox2);
    //m_guiObjects.push_back(text1);
    //m_guiObjects.push_back(text2);
    m_guiObjects.push_back(drawFrameTime);

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
    for(const std::shared_ptr<Beryll::GUIObject>& go : m_guiObjects)
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
    // dont need update GUI objects after physics simulation
}

void PlayGUILayer::draw()
{
    for(const std::shared_ptr<Beryll::GUIObject>& go : m_guiObjects)
    {
        if(go->getIsEnabled())
        {
            go->draw();
        }
    }
}

void PlayGUILayer::playSound()
{
    for(const std::shared_ptr<Beryll::GUIObject>& go : m_guiObjects)
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