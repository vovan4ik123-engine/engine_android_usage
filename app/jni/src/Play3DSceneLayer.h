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
    void loadTestWorld();
    void loadLeraWorld();

    std::shared_ptr<Beryll::Shader> m_simpleSunLight;
    std::shared_ptr<Beryll::Shader> m_animSunLight;

    std::shared_ptr<Beryll::Shader> m_simpleSunLightShadowMap;
    std::shared_ptr<Beryll::Shader> m_animSunLightShadowMap;

    std::shared_ptr<Beryll::Shader> m_simpleSunLightShadowMapNormalMap;
    std::shared_ptr<Beryll::Shader> m_animSunLightShadowMapNormalMap;

    std::shared_ptr<PlayGUILayer> m_guiLayer;

    std::vector<std::shared_ptr<Beryll::SceneObject>> m_allSceneObjects;
    std::vector<std::shared_ptr<Beryll::CollidingSimpleObject>> m_allGroundObjects;
    std::vector<std::shared_ptr<Beryll::CollidingSimpleObject>> m_allSphereObjects;
    std::vector<std::shared_ptr<Beryll::BaseSimpleObject>> m_simpleObjectsForShadows;
    std::vector<std::shared_ptr<Beryll::BaseAnimatedObject>> m_allAnimatedObjects;
    std::shared_ptr<Beryll::CollidingSimplePlayer> m_player;
    std::unique_ptr<Beryll::SkyBox> m_skyBox;

    std::unique_ptr<Beryll::Texture> m_shadowMapTexture;

    std::unique_ptr<Beryll::Texture> m_groundNormalMapTexture;

    float m_lastFingerMovePosX = 0.0f;
    float m_lastFingerMovePosY = 0.0f;
    float m_angleXZ = 0.0f;
    float m_angleYZ = 0.0f;

    glm::vec3 m_cameraOffset = glm::vec3(1.0f, 0.0f, 0.0f);

    // draw shadow map debug
    //std::shared_ptr<Beryll::Shader> m_drawShadowMap;
    //void drawShadowMap();
    //uint32_t quadVAO = 0;
    //uint32_t quadVBO = 0;
};