#pragma once

#include "EngineHeaders.h"

class PauseGUILayer : public Beryll::Layer
{
public:
    PauseGUILayer();
    ~PauseGUILayer();

    virtual void updateBeforePhysics() override; // interactions between gameobjects here
    virtual void updateAfterPhysics() override;
    virtual void draw() override; // draw gameobjects here
    virtual void playSound() override; // play sound of gameobjects here and music on layer

    std::shared_ptr<Beryll::Button> buttonResumeGame;
};