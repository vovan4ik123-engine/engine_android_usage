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
        static uint32_t particlesCount = 0;

        if(Beryll::TimeStep::getMillisecFromStart() > lastUpdate + 200)
        {
            frameTime = Beryll::GameLoop::getFrameTime();
            FPS = Beryll::GameLoop::getFPS();
            drawTime = Beryll::GameLoop::drawTime;
            calcTime = Beryll::GameLoop::calcTime;
            particlesCount = Beryll::ParticleSystem::getInstance()->getActiveCount();

            lastUpdate = Beryll::TimeStep::getMillisecFromStart();
        }

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.0f, 0.0f, 0.0f, 1.0f });

        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::Begin("FrameTime", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                           ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);


        ImGui::Text("Frame time:%.3u, FPS:%.3u, Patricles:%.5u", frameTime, FPS, particlesCount);
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

    buttonJump = std::make_shared<Beryll::Button>("Jump", 80, 80, 20, 20);
    buttonJump->setColor(0.4f, 0.4f, 0.4f, 0.3f);
    buttonJump->setClickedColor(0.4f, 0.4f, 0.4f, 0.0f);

    buttonMove = std::make_shared<Beryll::Button>("Move", -1, 80, 20, 20, true);
    buttonMove->setColor(0.4f, 0.4f, 0.4f, 0.3f);
    buttonMove->setClickedColor(0.4f, 0.4f, 0.4f, 0.0f);

    buttonPause = std::make_shared<Beryll::Button>("Pause", 80, -1, 20, 20);
    buttonPause->setColor(0.4f, 0.4f, 0.4f, 0.3f);
    buttonPause->setClickedColor(0.4f, 0.4f, 0.4f, 0.0f);

    buttonParticles = std::make_shared<Beryll::Button>("Particles", 80, 60, 20, 20, true);
    buttonParticles->setColor(0.9f, 0.3f, 0.0f, 0.3f);
    buttonParticles->setClickedColor(0.9f, 0.0f, 0.0f, 0.3f);

    //text1 = std::make_shared<Beryll::Text>("Text test1", 30, 50);

    //text2 = std::make_shared<Beryll::Text>("Text test 2 \nnew line here", 30, 60, true);
    //text2->setBackgroundColor(0.0f, 0.0f, 0.4f, 0.7f);
    //text2->setFontColor(0.0f, 1.0f, 0.0f, 1.0f);

    sliderCamera = std::make_shared<Beryll::Slider>("Cam", 0, 8, 10, 3, true);
    sliderCamera->setFontColor(0.0f, 0.0f, 0.0f, 1.0f);
    sliderCamera->setTextBackgroundColor(1.0f, 1.0f, 1.0f, 0.3f);
    sliderCamera->setDragAreaColor(1.0f, 0.0f, 0.0f, 1.0f);
    sliderCamera->setSliderGrabColor(0.0f, 0.0f, 0.0f, 1.0f);

    sliderSunPower = std::make_shared<Beryll::Slider>("Sun", 0, 13, 10, 3, true);
    sliderSunPower->setFontColor(0.0f, 0.0f, 0.0f, 1.0f);
    sliderSunPower->setTextBackgroundColor(1.0f, 1.0f, 1.0f, 0.3f);
    sliderSunPower->setDragAreaColor(1.0f, 0.0f, 0.0f, 1.0f);
    sliderSunPower->setSliderGrabColor(0.0f, 0.0f, 0.0f, 1.0f);

    sliderParticles = std::make_shared<Beryll::Slider>("Particles", 0, 18, 10, 3, true);
    sliderParticles->setFontColor(0.0f, 0.0f, 0.0f, 1.0f);
    sliderParticles->setTextBackgroundColor(1.0f, 1.0f, 1.0f, 0.3f);
    sliderParticles->setDragAreaColor(1.0f, 0.0f, 0.0f, 1.0f);
    sliderParticles->setSliderGrabColor(0.0f, 0.0f, 0.0f, 1.0f);

    sliderParticlesLifeTime = std::make_shared<Beryll::Slider>("LifeTime", 0, 23, 10, 3, true);
    sliderParticlesSize = std::make_shared<Beryll::Slider>("Size", 0, 28, 10, 3, true);

    checkBoxParticlesCubes = std::make_shared<Beryll::CheckBox>("Cubes", 0, 33);
    checkBoxParticlesCubes->setSoundWAV("sounds/gren2.wav");

    drawFrameTime = std::make_shared<Beryll::DrawAnyFunction>(showFrameTime);

    // add GUIObjects to MyLayer
    //m_guiObjects.push_back(guiDemo);
    m_guiObjects.push_back(buttonJump);
    m_guiObjects.push_back(buttonMove);
    m_guiObjects.push_back(buttonParticles);
    m_guiObjects.push_back(buttonPause);
    m_guiObjects.push_back(sliderCamera);
    m_guiObjects.push_back(sliderSunPower);
    m_guiObjects.push_back(sliderParticles);
    m_guiObjects.push_back(sliderParticlesLifeTime);
    m_guiObjects.push_back(sliderParticlesSize);
    m_guiObjects.push_back(checkBoxParticlesCubes);
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

    if(0 && !Beryll::SoundsManager::getIsBackgroundMP3Started())
    {
        Beryll::SoundsManager::startBackgroundMP3("sounds/MP3/JOY.mp3");
    }
    else if(0 && Beryll::SoundsManager::getIsBackgroundMP3Playing())
    {
        Beryll::SoundsManager::pauseBackgroundMP3();
    }
    else if(0 && Beryll::SoundsManager::getIsBackgroundMP3Paused())
    {
        Beryll::SoundsManager::resumeBackgroundMP3();
    }

    if(0 && !Beryll::SoundsManager::getIsBackgroundMP3Started())
    {
        Beryll::SoundsManager::startBackgroundMP3("sounds/MP3/Fancy.mp3");
    }
    else if(0 && Beryll::SoundsManager::getIsBackgroundMP3Playing())
    {
        Beryll::SoundsManager::pauseBackgroundMP3();
    }
    else if(0 && Beryll::SoundsManager::getIsBackgroundMP3Paused())
    {
        Beryll::SoundsManager::resumeBackgroundMP3();
    }

    if(0)
    {
        Beryll::SoundsManager::stopBackgroundMP3();
    }
}