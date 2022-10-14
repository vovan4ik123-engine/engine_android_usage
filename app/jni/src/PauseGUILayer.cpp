#include "PauseGUILayer.h"

PauseGUILayer::PauseGUILayer()
{
    m_ID = Beryll::LayerID::PAUSE_GUI;

    buttonResumeGame = std::make_shared<Beryll::Button>("Resume", 40, 40, 20, 10);

    m_guiObjects.push_back(buttonResumeGame);
}

PauseGUILayer::~PauseGUILayer()
{

}

void PauseGUILayer::updateBeforePhysics()
{
    for(const std::shared_ptr<Beryll::GUIObject>& go : m_guiObjects)
    {
        if(go->getIsEnabled())
        {
            go->updateBeforePhysics();
        }
    }

    if(buttonResumeGame->getIsPressed())
    {
        Beryll::Physics::enableSimulation();
        Beryll::GameStateMachine::popState();
    }
}

void PauseGUILayer::updateAfterPhysics()
{

}

void PauseGUILayer::draw()
{
    for(const std::shared_ptr<Beryll::GUIObject>& go : m_guiObjects)
    {
        if(go->getIsEnabled())
        {
            go->draw();
        }
    }
}

void PauseGUILayer::playSound()
{
    for(const std::shared_ptr<Beryll::GUIObject>& go : m_guiObjects)
    {
        if(go->getIsEnabled())
        {
            go->playSound();
        }
    }
}