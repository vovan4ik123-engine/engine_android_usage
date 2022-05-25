#pragma once

#include "EngineHeaders.h"

#include "PauseGUILayer.h"

class PlayGUILayer : public Beryll::Layer
{
public:
    PlayGUILayer();
    ~PlayGUILayer();

    virtual void updateBeforePhysics() override; // interactions between gameobjects here
    virtual void updateAfterPhysics() override;
    virtual void draw() override; // draw gameobjects here
    virtual void playSound() override; // play sound of gameobjects here and music on layer

    std::shared_ptr<Beryll::DemoImGUI> guiDemo;

    std::shared_ptr<Beryll::Button> buttonUp;
    std::shared_ptr<Beryll::Button> buttonMiddle;
    std::shared_ptr<Beryll::Button> buttonDown;
    std::shared_ptr<Beryll::Button> buttonResetCube;
    std::shared_ptr<Beryll::Button> buttonMove;
    std::shared_ptr<Beryll::Button> buttonPause;

    std::shared_ptr<Beryll::CheckBox> checkBox1;
    std::shared_ptr<Beryll::CheckBox> checkBox2;

    std::shared_ptr<Beryll::Text> text1;
    std::shared_ptr<Beryll::Text> text2;

    std::shared_ptr<Beryll::DrawAnyFunction> drawFrameTime;

    // pause
    std::shared_ptr<PauseGUILayer> pauseGUILayer;
    std::shared_ptr<Beryll::GameState> pauseState;
};
