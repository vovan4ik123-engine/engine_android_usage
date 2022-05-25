#pragma once

#include "EngineHeaders.h"
#include "PlayGUILayer.h"

class Play3DSceneLayer : public Beryll::Layer
{
public:
    Play3DSceneLayer(std::shared_ptr<PlayGUILayer> guiLayer);
    ~Play3DSceneLayer();

    virtual void updateBeforePhysics() override; // interactions between gameobjects here
    virtual void updateAfterPhysics() override;
    virtual void draw() override; // draw gameobjects here
    virtual void playSound() override; // play sound of gameobjects here and music on layer

private:
    //std::shared_ptr<Beryll::SimpleObject> simpleCubeSphere;
    //std::shared_ptr<Beryll::AnimatedObject> enemyMan;
    //std::shared_ptr<Beryll::CollidingAnimatedObject> worm;
    //std::shared_ptr<Beryll::CollidingSimpleObject> collPlane;
    //std::shared_ptr<Beryll::CollidingSimpleObject> collWall;
    //std::shared_ptr<Beryll::CollidingSimpleObject> player;
    //std::vector<std::shared_ptr<Beryll::CollidingAnimatedObject>> collWarms;

    std::shared_ptr<Beryll::CollidingSimpleObject> testCube;
    std::shared_ptr<Beryll::CollidingSimpleObject> ground1;
    std::shared_ptr<Beryll::CollidingSimpleObject> ground2;

    std::shared_ptr<PlayGUILayer> m_guiLayer;

    float m_lastFingerMovePosX = 0.0f;
    float m_lastFingerMovePosY = 0.0f;
    float m_angleXZ = 0.0f;
    float m_angleYZ = 0.0f;

    glm::vec3 m_cameraOffset = glm::vec3(1.0f, 0.0f, 0.0f);
};