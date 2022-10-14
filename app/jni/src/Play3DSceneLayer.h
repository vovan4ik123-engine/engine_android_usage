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
    void createShaders();
    std::shared_ptr<Beryll::Shader> m_simpleSunLight;
    std::shared_ptr<Beryll::Shader> m_animSunLight;
    std::shared_ptr<Beryll::Shader> m_shadowMapSimple;
    std::shared_ptr<Beryll::Shader> m_shadowMapAnim;

    std::shared_ptr<PlayGUILayer> m_guiLayer;

    std::vector<std::shared_ptr<Beryll::SceneObject>> m_sceneObjects;
    std::vector<std::shared_ptr<Beryll::BaseSimpleObject>> m_allSimpleObjects;
    std::vector<std::shared_ptr<Beryll::BaseAnimatedObject>> m_allAnimatedObjects;
    std::shared_ptr<Beryll::CollidingSimplePlayer> m_player;
    std::unique_ptr<Beryll::SkyBox> m_skyBox;

    float m_lastFingerMovePosX = 0.0f;
    float m_lastFingerMovePosY = 0.0f;
    float m_angleXZ = 0.0f;
    float m_angleYZ = 0.0f;

    glm::vec3 m_cameraOffset = glm::vec3(0.0f, 1.0f, 1.0f);
};