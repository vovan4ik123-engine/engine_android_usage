#include "Play3DSceneLayer.h"

#include <GLES3/gl31.h>
#include <GLES3/gl3ext.h>

Play3DSceneLayer::Play3DSceneLayer(std::shared_ptr<PlayGUILayer> guiLayer)
{
    m_ID = Beryll::LayerID::PLAY_SCENE;

    m_guiLayer = std::move(guiLayer);

    m_guiLayer->sliderCamera->setValue(0.3f);
    m_guiLayer->sliderParticlesLifeTime->setValue(0.5f);
    m_guiLayer->sliderParticlesSize->setValue(0.5f);
    m_guiLayer->sliderParticles->setValue(0.5f);

    m_allSceneObjects.reserve(3000);
    m_allGroundObjects.reserve(150);
    m_allSphereObjects.reserve(150);
    m_allSimpleObjects.reserve(3000);
    m_allAnimatedObjects.reserve(3000);

//    simpleCubeSphere = std::make_shared<Beryll::SimpleObject>("models/cube_text.dae",
//                                                              "shaders/GLES/default/Simple.vert",
//                                                              "shaders/GLES/default/Simple.frag",
//                                                              "diffuseTexture");
//
//    enemyMan = std::make_shared<Beryll::AnimatedObject>("models/garbage/model.dae",
//                                                        "shaders/GLES/default/Animation.vert",
//                                                        "shaders/GLES/default/Animation.frag",
//                                                        "diffuseTexture");

//    collPlane = std::make_shared<Beryll::CollidingSimpleObject>("models/garbage/SimCollGround001_0kg.dae",
//                                                                0,
//                                                                true,
//                                                                Beryll::CollisionFlags::STATIC,
//                                                                Beryll::CollisionGroups::STATIC_ENVIRONMENT,
//                                                                Beryll::CollisionGroups::PLAYER | Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT | Beryll::CollisionGroups::CAMERA,
//                                                                "shaders/GLES/default/Simple.vert",
//                                                                "shaders/GLES/default/Simple.frag",
//                                                                "diffuseTexture");

//    std::make_shared<Beryll::CollidingAnimatedObject>("models/collisionsAnim.dae",
//                                                      15,
//                                                      false,
//                                                      Beryll::CollisionFlags::DYNAMIC,
//                                                      Beryll::CollisionGroups::CUBE,
//                                                      Beryll::CollisionGroups::STATIC_ENVIRONMENT | Beryll::CollisionGroups::WALL |
//                                                      Beryll::CollisionGroups::CUBE | Beryll::CollisionGroups::PLAYER,
//                                                      "shaders/GLES/default/Animation.vert",
//                                                      "shaders/GLES/default/Animation.frag",
//                                                      "diffuseTexture"));

    //auto simpleCubeSphere = std::make_shared<Beryll::SimpleObject>("models/garbage/NotTriangulated.fbx");

    for(int i = 0; i < 1000; ++i)
    {
        auto animatedObject = std::make_shared<Beryll::AnimatedObject>("models/garbage/model.dae");

        m_allSceneObjects.push_back(animatedObject);
        m_allAnimatedObjects.push_back(animatedObject);
        animatedObject->setOrigin(glm::vec3(Beryll::RandomGenerator::getInt(900),
                                              20.0f,
                                              -Beryll::RandomGenerator::getInt(900)));

    }
    for(int i = 0; i < 1000; ++i)
    {
        auto testBall = std::make_shared<Beryll::CollidingSimpleObject>("models/garbage/SphereSphere.fbx",
                                                                        5.0f,
                                                                        true,
                                                                        Beryll::CollisionFlags::DYNAMIC,
                                                                        Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT,
                                                                        Beryll::CollisionGroups::STATIC_ENVIRONMENT | Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT | Beryll::CollisionGroups::PLAYER);

        m_allSceneObjects.push_back(testBall);
        m_allSimpleObjects.push_back(testBall);
        m_allSphereObjects.push_back(testBall);
        testBall->setOrigin(glm::vec3(Beryll::RandomGenerator::getInt(900),
                                      20.0f,
                                      -Beryll::RandomGenerator::getInt(900)));
    }

    std::string modelName;
    for(int i = 1; i <= 100; ++i)
    {
        if(i < 10)
        {
            modelName = "models/TestWorld/00";
            modelName += std::to_string(i);
            modelName += ".dae";
        }
        else if(i >= 10 && i < 100)
        {
            modelName = "models/TestWorld/0";
            modelName += std::to_string(i);
            modelName += ".dae";
        }
        else if(i == 100)
        {
            modelName = "models/TestWorld/100.dae";
        }
        auto testWorld = std::make_shared<Beryll::CollidingSimpleObject>(modelName.c_str(),
                                                                         0,
                                                                         true,
                                                                         Beryll::CollisionFlags::STATIC,
                                                                         Beryll::CollisionGroups::STATIC_ENVIRONMENT | Beryll::CollisionGroups::GROUND,
                                                                         Beryll::CollisionGroups::PLAYER | Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT | Beryll::CollisionGroups::CAMERA);
        m_allSceneObjects.push_back(testWorld);
        m_allSimpleObjects.push_back(testWorld);
        m_allGroundObjects.push_back(testWorld);
    }

    m_player = std::make_shared<Beryll::CollidingSimplePlayer>("models/garbage/CapsuleCapsule.dae",
                                                             10.0f,
                                                             true,
                                                             Beryll::CollisionFlags::DYNAMIC,
                                                             Beryll::CollisionGroups::PLAYER,
                                                             Beryll::CollisionGroups::STATIC_ENVIRONMENT | Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT);

    m_allSceneObjects.push_back(m_player);
    m_allSimpleObjects.push_back(m_player);

    m_player->setOrigin(glm::vec3(0.0f, 7.0f,0.0f));
    m_player->jumpExtendTime = 1.0f;
    m_player->startJumpPower = 20.0f;
    m_player->startFallingPower = 20.0f;

    Beryll::Camera::setCameraPos(m_player->getOrigin() + glm::vec3(40.0f, 0.0f, 0.0f));
    Beryll::Camera::setCameraFront(m_player->getOrigin());

    m_skyBox = Beryll::Renderer::createSkyBox("skybox");

    createShaders();

    m_shadowMapTexture = Beryll::Renderer::createShadowMapTexture(2048, 2048);
    m_groundNormalMapTexture = Beryll::Renderer::createTexture("models/TestWorld/Ground001-100NormalMap.png", Beryll::TextureType::NORMAL_MAP_TEXTURE);

    std::function<void(std::vector<std::shared_ptr<Beryll::SceneObject>>&, int, int)> disableModels =
            [this](std::vector<std::shared_ptr<Beryll::SceneObject>>& v, int begin, int end) -> void // -> void = return type
            {
                for(int i = begin; i < end; ++i)
                {
                    if (glm::distance(m_player->getOrigin(), v[i]->getOrigin()) > 80.0f)
                    {
                        v[i]->disableOnScene();
                        v[i]->disableCollisionMesh();
                    }
                }
            };

    Beryll::AsyncRun::Run<std::shared_ptr<Beryll::SceneObject>>(m_allSceneObjects, disableModels);
}

Play3DSceneLayer::~Play3DSceneLayer()
{

}

void Play3DSceneLayer::updateBeforePhysics()
{
    // 1. react on user input

    if(m_guiLayer->buttonMove->getIsPressed()) // repeat action
    {
        m_player->move(Beryll::MoveDirection::FORWARD);

        glm::vec3 pos = m_player->getOrigin();
        pos.y += 4.0f;
        if(m_guiLayer->checkBoxParticlesCubes->getIsChecked())
        {
            Beryll::ParticleSystem::getInstance()->EmitCubesFromCenter(m_guiLayer->sliderParticles->getValue() * 50.0f, m_guiLayer->sliderParticlesLifeTime->getValue() * 25.0f,
                                                                       m_guiLayer->sliderParticlesSize->getValue() * 3.0f, m_guiLayer->sliderParticlesSize->getValue() * 3.0f,
                                                                       {0.9f, 0.34f, 0.13f, 1.0f}, {0.0f, 0.0f, 0.0f, 0.4f},
                                                                       pos, {0.0f, 0.0f, 0.0f}, 2.5f);
        }
        else
        {
            Beryll::ParticleSystem::getInstance()->EmitQuadsFromCenter(m_guiLayer->sliderParticles->getValue() * 50.0f, m_guiLayer->sliderParticlesLifeTime->getValue() * 25.0f,
                                                                       m_guiLayer->sliderParticlesSize->getValue() * 3.0f, m_guiLayer->sliderParticlesSize->getValue() * 3.0f,
                                                                       {0.9f, 0.34f, 0.13f, 1.0f}, {0.0f, 0.0f, 0.0f, 0.4f},
                                                                       pos, {0.0f, 0.0f, 0.0f}, 2.5f);
        }
    }

    if(m_guiLayer->buttonJump->getIsPressed()) // one action
    {
        m_player->jump();
    }

    if(m_guiLayer->buttonExplosion->getIsPressed())
    {
        glm::vec3 pos = m_player->getOrigin();
        pos.y += 4.0f;

        if(m_guiLayer->checkBoxParticlesCubes->getIsChecked())
        {
            Beryll::ParticleSystem::getInstance()->EmitCubesExplosion(3000.0f, m_guiLayer->sliderParticlesLifeTime->getValue() * 2.0f,
                                                                      m_guiLayer->sliderParticlesSize->getValue() * 2.0f, m_guiLayer->sliderParticlesSize->getValue() * 1.0f,
                                                                      {10.0f, 5.0f, 10.0f},
                                                                      {1.0f, 0.0f, 0.0f, 1.0f}, {0.9f, 0.34f, 0.13f, 0.4f},
                                                                      pos, {0.0f, 0.0f, 0.0f}, 20.0f);

            Beryll::ParticleSystem::getInstance()->EmitCubesExplosion(3000.0f, m_guiLayer->sliderParticlesLifeTime->getValue() * 4.0f,
                                                                      m_guiLayer->sliderParticlesSize->getValue() * 1.0f, m_guiLayer->sliderParticlesSize->getValue() * 2.0f,
                                                                      {8.0f, 8.0f, 8.0f},
                                                                      {0.3f, 0.3f, 0.3f, 1.0f}, {0.0f, 0.0f, 0.0f, 0.4f},
                                                                      pos, {0.0f, 15.0f, 0.0f}, 8.0f);
        }
        else
        {
            Beryll::ParticleSystem::getInstance()->EmitQuadsExplosion(3000.0f, m_guiLayer->sliderParticlesLifeTime->getValue() * 2.0f,
                                                                      m_guiLayer->sliderParticlesSize->getValue() * 2.0f, m_guiLayer->sliderParticlesSize->getValue() * 1.0f,
                                                                      {10.0f, 5.0f, 10.0f},
                                                                      {1.0f, 0.0f, 0.0f, 1.0f}, {0.9f, 0.34f, 0.13f, 0.4f},
                                                                      pos, {0.0f, 0.0f, 0.0f}, 20.0f);

            Beryll::ParticleSystem::getInstance()->EmitQuadsExplosion(3000.0f, m_guiLayer->sliderParticlesLifeTime->getValue() * 4.0f,
                                                                      m_guiLayer->sliderParticlesSize->getValue() * 1.0f, m_guiLayer->sliderParticlesSize->getValue() * 2.0f,
                                                                      {8.0f, 8.0f, 8.0f},
                                                                      {0.3f, 0.3f, 0.3f, 1.0f}, {0.0f, 0.0f, 0.0f, 0.4f},
                                                                      pos, {0.0f, 15.0f, 0.0f}, 8.0f);
        }
    }

    std::vector<Beryll::Finger> fingers = Beryll::EventHandler::getFingers();

    for(const Beryll::Finger& f : fingers)
    {
        if(f.handled) { continue; }

        if(f.downEvent)
        {
            m_lastFingerMovePosX = f.SDL2ScreenPos.x;
            m_lastFingerMovePosY = f.SDL2ScreenPos.y;
            //enemyWorm->setAnimation("Armature|Cube1_jump");
            break;
        }
        else
        {
            float deltaX = f.SDL2ScreenPos.x - m_lastFingerMovePosX;
            float deltaY = f.SDL2ScreenPos.y - m_lastFingerMovePosY;

            // euler angles * distance
            m_cameraOffset.x = (glm::cos(glm::radians(m_angleXZ)) * glm::cos(glm::radians(m_angleYZ)));
            m_cameraOffset.y = glm::sin(glm::radians(m_angleYZ));
            m_cameraOffset.z = (glm::sin(glm::radians(m_angleXZ)) * glm::cos(glm::radians(m_angleYZ)));

            m_angleXZ += deltaX;
            m_angleYZ += deltaY;
            if(m_angleYZ < -88.0f) m_angleYZ = -88.0f;
            if(m_angleYZ > 88.0f) m_angleYZ = 88.0f;
            m_lastFingerMovePosX = f.SDL2ScreenPos.x;
            m_lastFingerMovePosY = f.SDL2ScreenPos.y;
            break;
        }
    }

    // 2. prepare for simulation

    std::function<void(std::vector<std::shared_ptr<Beryll::SceneObject>>&, int, int)> updateBeforePhysics =
            [](std::vector<std::shared_ptr<Beryll::SceneObject>>& v, int begin, int end) -> void // -> void = return type
            {
                for(int i = begin; i < end; ++i)
                {
                    if(v[i]->getIsEnabledOnScene())
                    {
                        v[i]->updateBeforePhysics();
                    }
                }
            };

    Beryll::AsyncRun::Run(m_allSceneObjects, updateBeforePhysics);

    //BR_INFO("m_player->getOrigin() 2 XYZ %f, %f. %f", m_player->getOrigin().x, m_player->getOrigin().y, m_player->getOrigin().z);
    //BR_INFO("m_allSphereObjects[0]->getOrigin() 2 XYZ %f, %f. %f", m_allSphereObjects[0]->getOrigin().x, m_allSphereObjects[0]->getOrigin().y, m_allSphereObjects[0]->getOrigin().z);
//    int numActive = 0;
//    for(const std::shared_ptr<Beryll::SceneObject>& obj : m_allSceneObjects)
//    {
//        if(obj->getIsEnabled())
//        {
//            ++numActive;
//        }
//    }
//    BR_INFO("Active number:%d", numActive);
}

void Play3DSceneLayer::updateAfterPhysics()
{
    // 1. let objects update themselves based on physics simulation
    std::function<void(std::vector<std::shared_ptr<Beryll::SceneObject>>&, int, int)> updateAfterPhysics =
            [](std::vector<std::shared_ptr<Beryll::SceneObject>>& v, int begin, int end) -> void // -> void = return type
            {
                for(int i = begin; i < end; ++i)
                {
                    if(v[i]->getIsEnabledOnScene())
                    {
                        v[i]->updateAfterPhysics();
                    }
                }
            };

    Beryll::AsyncRun::Run(m_allSceneObjects, updateAfterPhysics);

    //BR_INFO("m_player->getOrigin() XYZ %f, %f. %f", m_player->getOrigin().x, m_player->getOrigin().y, m_player->getOrigin().z);

    // 2. resolve collisions
    //if(Beryll::Physics::getIsCollision(collWall->getID(), m_player->getID()))
    //    BR_INFO("Player collising with wall");
    //if(Beryll::Physics::getIsCollision(collPlane->getID(), m_player->getID()))
    //   BR_INFO("Player collising with ground");

    // 3. use objects updated position
//    Beryll::RayClosestHit rayClosestHit = Beryll::Physics::castRayClosestHit(m_player->getOrigin(),
//                                                                          m_player->getOrigin() + m_cameraOffset,
//                                                                          Beryll::CollisionGroups::CAMERA,
//                                                                          Beryll::CollisionGroups::STATIC_ENVIRONMENT | Beryll::CollisionGroups::WALL);
//    if(rayClosestHit)
//        Beryll::Camera::setCameraPos(rayClosestHit.hitPoint);
//    else
//        Beryll::Camera::setCameraPos(m_player->getOrigin() + m_cameraOffset);
//
//  Beryll::Camera::setCameraFront(m_player->getOrigin());

    Beryll::Camera::setCameraPos(m_player->getOrigin() + m_cameraOffset * (m_guiLayer->sliderCamera->getValue() * 160.0f + 5.0f));
    Beryll::Camera::setCameraFront(m_player->getOrigin());
}

void Play3DSceneLayer::draw()
{
    std::function<void(std::vector<std::shared_ptr<Beryll::SceneObject>>&, int, int)> prepareForDraw =
            [this](std::vector<std::shared_ptr<Beryll::SceneObject>>& v, int begin, int end) -> void // -> void = return type
            {
                for(int i = begin; i < end; ++i)
                {
                    if(Beryll::Physics::getIsCollisionGroupContainsOther(v[i]->getCollisionGroup(),Beryll::CollisionGroups::GROUND))
                    {
                        // if piece of ground is 100x100 and origin is in center
                        // 110.0f from m_player origin for max 5 ground pieces enabled
                        // 155.0f from m_player origin for max 9 ground pieces enabled
                        if(glm::distance(m_player->getOrigin(), v[i]->getOrigin()) < 155.0f)
                        {
                            v[i]->enableOnScene();
                            v[i]->enableCollisionMesh();
                        }
                        else
                        {
                            v[i]->disableOnScene();
                            v[i]->disableCollisionMesh();
                        }
                    }
                    else
                    {
                        if(Beryll::Camera::getIsSeeObject(v[i]->getOrigin()))
                        {
                            v[i]->enableOnScene();
                            v[i]->enableCollisionMesh();

                            if(Beryll::Physics::getIsCollisionGroupContainsOther(v[i]->getCollisionGroup(),Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT))
                            {
                                if(glm::distance(m_player->getOrigin(), v[i]->getOrigin()) > 80.0f)
                                {
                                    v[i]->disableCollisionMesh();
                                }
                            }
                        }
                        else
                        {
                            v[i]->disableOnScene();
                            v[i]->disableCollisionMesh();
                        }
                    }
                }
            };

    Beryll::AsyncRun::Run(m_allSceneObjects, prepareForDraw);

    static glm::vec3 sunPos = glm::normalize(glm::vec3{1.0f, 40.0f, 0.0f}) * 50.0f;
    static glm::vec3 sunLightDir = glm::normalize(glm::vec3(0.0f) - sunPos);
    static float sunAngle = 0.0f;
    static uint32_t sunUpdatedTime = 0;
    if(Beryll::TimeStep::getMillisecFromStart() > sunUpdatedTime + 20)
    {
        // update sun
        sunAngle += 0.5f;
        if(sunAngle >= 360.0f) sunAngle = 0.0f;
        sunUpdatedTime = Beryll::TimeStep::getMillisecFromStart();

        //BR_INFO("sunPos XYZ %f, %f. %f", sunPos.x, sunPos.y, sunPos.z);
    }

    sunPos = glm::vec3(glm::cos(glm::radians(sunAngle)) * 50.0f, 50.0f, glm::sin(glm::radians(sunAngle)) * 50.0f);
    sunPos = m_player->getOrigin() + sunPos;
    sunLightDir = glm::normalize(m_player->getOrigin() - sunPos);


    glm::mat4 lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 4.0f, 200.0f);
    glm::mat4 lightView = glm::lookAt(sunPos,m_player->getOrigin(), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 VPLightMatrix = lightProjection * lightView;

    m_shadowMapTexture->drawIntoShadowMap(m_allSimpleObjects,
                                          m_allAnimatedObjects,
                                          VPLightMatrix);

    m_shadowMapTexture->bind();

    //m_drawShadowMap->bind();
    //drawShadowMap();

    glm::mat3 normalMatrix{1.0f};
    const glm::vec3 cameraPos = Beryll::Camera::getCameraPos();
    const float ambientLight = glm::min(m_guiLayer->sliderSunPower->getValue(), 0.3f);
    const float specularLightStrength = 0.3f + m_guiLayer->sliderSunPower->getValue() * 1.5f;

    m_simpleSunLightShadowMap->bind();
    m_simpleSunLightShadowMap->set3Float("sunLightDir", sunLightDir);
    m_simpleSunLightShadowMap->set3Float("cameraPos", cameraPos);
    m_simpleSunLightShadowMap->set1Float("ambientLight", ambientLight);
    m_simpleSunLightShadowMap->set1Float("specularLightStrength", specularLightStrength);

    m_simpleSunLightShadowMap->setMatrix4x4Float("MVPMatrix", Beryll::Camera::getViewProjection() * m_player->getModelMatrix());
    m_simpleSunLightShadowMap->setMatrix4x4Float("MVPLightMatrix", VPLightMatrix * m_player->getModelMatrix());
    m_simpleSunLightShadowMap->setMatrix4x4Float("modelMatrix", m_player->getModelMatrix());
    m_simpleSunLightShadowMap->setMatrix3x3Float("normalMatrix", glm::mat3(m_player->getModelMatrix()));

    m_player->useInternalShader = false;
    m_player->useInternalTextures = true;
    m_player->draw();

    m_simpleSunLightShadowMapNormalMap->bind();
    m_simpleSunLightShadowMapNormalMap->set3Float("sunLightDir", sunLightDir);
    m_simpleSunLightShadowMapNormalMap->set3Float("cameraPos", cameraPos);
    m_simpleSunLightShadowMapNormalMap->set1Float("ambientLight", ambientLight);
    m_simpleSunLightShadowMapNormalMap->set1Float("specularLightStrength", specularLightStrength);

    for(const std::shared_ptr<Beryll::CollidingSimpleObject>& obj : m_allSphereObjects)
    {
        if(obj->getIsEnabledOnScene())
        {
            m_simpleSunLightShadowMapNormalMap->setMatrix4x4Float("MVPMatrix", Beryll::Camera::getViewProjection() * obj->getModelMatrix());
            m_simpleSunLightShadowMapNormalMap->setMatrix4x4Float("MVPLightMatrix", VPLightMatrix * obj->getModelMatrix());
            m_simpleSunLightShadowMapNormalMap->setMatrix4x4Float("modelMatrix", obj->getModelMatrix());
            m_simpleSunLightShadowMapNormalMap->setMatrix3x3Float("normalMatrix", glm::mat3(obj->getModelMatrix()));

            obj->useInternalShader = false;
            obj->useInternalTextures = true;
            obj->draw();
        }
    }

    m_groundNormalMapTexture->bind();

    m_simpleSunLightShadowMapNormalMap->bind();
    m_simpleSunLightShadowMapNormalMap->set3Float("sunLightDir", sunLightDir);
    m_simpleSunLightShadowMapNormalMap->set3Float("cameraPos", cameraPos);
    m_simpleSunLightShadowMapNormalMap->set1Float("ambientLight", ambientLight);
    m_simpleSunLightShadowMapNormalMap->set1Float("specularLightStrength", specularLightStrength);

    for(const std::shared_ptr<Beryll::CollidingSimpleObject>& obj : m_allGroundObjects)
    {
        if(obj->getIsEnabledOnScene())
        {
            m_simpleSunLightShadowMapNormalMap->setMatrix4x4Float("MVPMatrix", Beryll::Camera::getViewProjection() * obj->getModelMatrix());
            m_simpleSunLightShadowMapNormalMap->setMatrix4x4Float("MVPLightMatrix", VPLightMatrix * obj->getModelMatrix());
            m_simpleSunLightShadowMapNormalMap->setMatrix4x4Float("modelMatrix", obj->getModelMatrix());
            m_simpleSunLightShadowMapNormalMap->setMatrix3x3Float("normalMatrix", glm::mat3(obj->getModelMatrix()));

            obj->useInternalShader = false;
            obj->useInternalTextures = true;
            obj->draw();
        }
    }

    m_groundNormalMapTexture->bind();

    m_animSunLightShadowMap->bind();
    m_animSunLightShadowMap->set3Float("sunLightDir", sunLightDir);
    m_animSunLightShadowMap->set3Float("cameraPos", cameraPos);
    m_animSunLightShadowMap->set1Float("ambientLight", ambientLight);
    m_animSunLightShadowMap->set1Float("specularLightStrength", specularLightStrength);

    std::string boneMatrixNameInShader;
    for(const std::shared_ptr<Beryll::BaseAnimatedObject>& obj : m_allAnimatedObjects)
    {
        if(obj->getIsEnabledOnScene())
        {
            m_animSunLightShadowMap->setMatrix4x4Float("MVPMatrix", Beryll::Camera::getViewProjection() * obj->getModelMatrix());
            m_animSunLightShadowMap->setMatrix4x4Float("MVPLightMatrix", VPLightMatrix * obj->getModelMatrix());
            m_animSunLightShadowMap->setMatrix4x4Float("modelMatrix", obj->getModelMatrix());
            m_animSunLightShadowMap->setMatrix3x3Float("normalMatrix", glm::mat3(obj->getModelMatrix()));

            uint32_t boneCount = obj->getBoneCount();
            for(int i = 0; i < boneCount; ++i)
            {
                boneMatrixNameInShader = "bonesMatrices[";
                boneMatrixNameInShader += std::to_string(i);
                boneMatrixNameInShader += "]";
                m_animSunLightShadowMap->setMatrix4x4Float(boneMatrixNameInShader.c_str(), obj->getBoneMatrices()[i].finalWorldTransform);
            }

            obj->useInternalShader = false;
            obj->useInternalTextures = true;
            obj->draw();
        }
    }

    // draw skybox then particles last
    m_skyBox->draw();
    Beryll::ParticleSystem::getInstance()->draw();
}

void Play3DSceneLayer::playSound()
{
    for(const std::shared_ptr<Beryll::SceneObject>& obj : m_allSceneObjects)
    {
        if(obj->getIsEnabledOnScene())
        {
            obj->playSound();
        }
    }
}

void Play3DSceneLayer::createShaders()
{
    m_drawShadowMap = Beryll::Renderer::createShader("shaders/GLES/shadowMap/DrawShadowMap.vert",
                                                     "shaders/GLES/shadowMap/DrawShadowMap.frag");
    m_drawShadowMap->bind();
    m_drawShadowMap->activateShadowMapTexture();

    m_simpleSunLight = Beryll::Renderer::createShader("shaders/GLES/SimpleSunLight.vert",
                                                      "shaders/GLES/SimpleSunLight.frag");
    m_simpleSunLight->bind();
    m_simpleSunLight->activateDiffuseTexture();

    m_animSunLight = Beryll::Renderer::createShader("shaders/GLES/AnimationSunLight.vert",
                                                    "shaders/GLES/AnimationSunLight.frag");
    m_animSunLight->bind();
    m_animSunLight->activateDiffuseTexture();

    m_simpleSunLightShadowMap = Beryll::Renderer::createShader("shaders/GLES/SimpleSunLightShadowMap.vert",
                                                               "shaders/GLES/SimpleSunLightShadowMap.frag");
    m_simpleSunLightShadowMap->bind();
    m_simpleSunLightShadowMap->activateDiffuseTexture();
    m_simpleSunLightShadowMap->activateShadowMapTexture();

    m_animSunLightShadowMap = Beryll::Renderer::createShader("shaders/GLES/AnimationSunLightShadowMap.vert",
                                                             "shaders/GLES/AnimationSunLightShadowMap.frag");
    m_animSunLightShadowMap->bind();
    m_animSunLightShadowMap->activateDiffuseTexture();
    m_animSunLightShadowMap->activateShadowMapTexture();

    m_simpleSunLightShadowMapNormalMap = Beryll::Renderer::createShader("shaders/GLES/SimpleSunLightShadowMapNormalMap.vert",
                                                                        "shaders/GLES/SimpleSunLightShadowMapNormalMap.frag");
    m_simpleSunLightShadowMapNormalMap->bind();
    m_simpleSunLightShadowMapNormalMap->activateDiffuseTexture();
    m_simpleSunLightShadowMapNormalMap->activateShadowMapTexture();
    m_simpleSunLightShadowMapNormalMap->activateNormalMapTexture();

    m_animSunLightShadowMapNormalMap = Beryll::Renderer::createShader("shaders/GLES/AnimationSunLightShadowMapNormalMap.vert",
                                                                      "shaders/GLES/AnimationSunLightShadowMapNormalMap.frag");
    m_animSunLightShadowMapNormalMap->bind();
    m_animSunLightShadowMapNormalMap->activateDiffuseTexture();
    m_animSunLightShadowMapNormalMap->activateShadowMapTexture();
    m_animSunLightShadowMapNormalMap->activateNormalMapTexture();
}

void Play3DSceneLayer::drawShadowMap()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}