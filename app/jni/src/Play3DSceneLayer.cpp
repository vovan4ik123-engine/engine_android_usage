#include "Play3DSceneLayer.h"

Play3DSceneLayer::Play3DSceneLayer(std::shared_ptr<PlayGUILayer> guiLayer)
{
    m_ID = Beryll::LayerID::PLAY_SCENE;

    m_guiLayer = std::move(guiLayer);

    m_guiLayer->sliderCamera->setValue(0.3f);

    m_sceneObjects.reserve(3000);
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

    for(int i = 0; i < 1000; ++i)
    {
//        auto animatedObject = std::make_shared<Beryll::AnimatedObject>("models/garbage/model.dae");
//
//        m_sceneObjects.push_back(animatedObject);
//        m_allAnimatedObjects.push_back(animatedObject);
//        animatedObject->setOrigin(glm::vec3(Beryll::RandomGenerator::getFastInt(0, 900),
//                                              20.0f,
//                                              -Beryll::RandomGenerator::getFastInt(0, 900)));

        auto animatedObject = std::make_shared<Beryll::AnimatedObject>("models/garbage/model.dae");

    }
    for(int i = 0; i < 1000; ++i)
    {
        auto testBall = std::make_shared<Beryll::CollidingSimpleObject>("models/garbage/SphereSphere.dae",
                                                                        5.0f,
                                                                        true,
                                                                        Beryll::CollisionFlags::DYNAMIC,
                                                                        Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT,
                                                                        Beryll::CollisionGroups::STATIC_ENVIRONMENT | Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT | Beryll::CollisionGroups::PLAYER);

        m_sceneObjects.push_back(testBall);
        m_allSimpleObjects.push_back(testBall);
        testBall->setOrigin(glm::vec3(Beryll::RandomGenerator::getFastInt(0, 900),
                                      20.0f,
                                      -Beryll::RandomGenerator::getFastInt(0, 900)));
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
        m_sceneObjects.push_back(testWorld);
        m_allSimpleObjects.push_back(testWorld);
    }

    m_player = std::make_shared<Beryll::CollidingSimplePlayer>("models/garbage/CapsuleCapsule.dae",
                                                             10.0f,
                                                             true,
                                                             Beryll::CollisionFlags::DYNAMIC,
                                                             Beryll::CollisionGroups::PLAYER,
                                                             Beryll::CollisionGroups::STATIC_ENVIRONMENT | Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT);

    m_sceneObjects.push_back(m_player);
    m_allSimpleObjects.push_back(m_player);

    m_player->setOrigin(glm::vec3(0.0f, 7.0f,0.0f));

    Beryll::Camera::setCameraPos(m_player->getOrigin() + glm::vec3(0.0f, 40.0f, 50.0f));
    Beryll::Camera::setCameraFront(m_player->getOrigin());

    m_skyBox = Beryll::Renderer::createSkyBox("skybox");

    createShaders();

    std::function<void(const std::vector<std::shared_ptr<Beryll::SceneObject>>&, int, int)> disableModels =
            [this](const std::vector<std::shared_ptr<Beryll::SceneObject>>& v, int begin, int end) -> void // -> void = return type
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

    Beryll::AsyncRun::Run(m_sceneObjects, disableModels);
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
    }

    if(m_guiLayer->buttonJump->getIsPressed()) // one action
    {
        m_player->jump();
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

    std::function<void(const std::vector<std::shared_ptr<Beryll::SceneObject>>&, int, int)> updateBeforePhysics =
            [](const std::vector<std::shared_ptr<Beryll::SceneObject>>& v, int begin, int end) -> void // -> void = return type
            {
                for(int i = begin; i < end; ++i)
                {
                    if(v[i]->getIsEnabledOnScene())
                    {
                        v[i]->updateBeforePhysics();
                    }
                }
            };

    Beryll::AsyncRun::Run(m_sceneObjects, updateBeforePhysics);

//    int numActive = 0;
//    for(const std::shared_ptr<Beryll::SceneObject>& so : m_sceneObjects)
//    {
//        if(so->getIsEnabled())
//        {
//            ++numActive;
//        }
//    }
//    BR_INFO("Active number:%d", numActive);
}

void Play3DSceneLayer::updateAfterPhysics()
{
    // 1. let objects update themselves based on physics simulation
    std::function<void(const std::vector<std::shared_ptr<Beryll::SceneObject>>&, int, int)> updateAfterPhysics =
            [](const std::vector<std::shared_ptr<Beryll::SceneObject>>& v, int begin, int end) -> void // -> void = return type
            {
                for(int i = begin; i < end; ++i)
                {
                    if(v[i]->getIsEnabledOnScene())
                    {
                        v[i]->updateAfterPhysics();
                    }
                }
            };

    Beryll::AsyncRun::Run(m_sceneObjects, updateAfterPhysics);

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
    std::function<void(const std::vector<std::shared_ptr<Beryll::SceneObject>>&, int, int)> prepareForDraw =
            [this](const std::vector<std::shared_ptr<Beryll::SceneObject>>& v, int begin, int end) -> void // -> void = return type
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

    Beryll::AsyncRun::Run(m_sceneObjects, prepareForDraw);

    // bind shaders use Renderer for draw objects with external shader
    // or draw objects themselves with internal shader

//    for(const std::shared_ptr<Beryll::SceneObject>& so : m_sceneObjects)
//    {
//        if(so->getIsEnabled())
//        {
//            so->draw(); // all draw calls must be called in one thread
//        }
//    }

    static glm::vec3 sunPos;
    static glm::vec3 sunLightDir;
    static float sunAngle = 0.0f;
    static uint32_t sunUpdatedTime = 0;
    if(Beryll::TimeStep::getMillisecFromStart() > sunUpdatedTime + 25)
    {
        // update sun
        sunAngle += 0.5f;
        BR_INFO("sunAngle %f", sunAngle);
        sunUpdatedTime = Beryll::TimeStep::getMillisecFromStart();
        sunPos = glm::vec3(glm::cos(glm::radians(sunAngle)) * 1000.0f, 500.0f, glm::sin(glm::radians(sunAngle)) * 1000.0f);
        sunLightDir = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - sunPos);
    }

    //BR_INFO("%f, %f. %f", sunLightDir.x, sunLightDir.y, sunLightDir.z);

    // Create shadow map based on light position and scene objects
    // create shadow map shaders
    // set sun position
    // set light perspective matrix

    //m_shadowMapSimple
    //m_shadowMapAnim

    glm::mat4 MVP{1.0f};
    glm::mat3 normalMatrix{1.0f};
    const glm::vec3 cameraPos = Beryll::Camera::getCameraPos();
    const float ambientLight = glm::min(m_guiLayer->sliderSunPower->getValue() + 0.1f, 0.5f);
    const float specularLightStrength = 0.4f + m_guiLayer->sliderSunPower->getValue() * 2.0f;

    m_animSunLight->bind();
    m_animSunLight->set3Float("sunLightDir", sunLightDir.x, sunLightDir.y, sunLightDir.z);
    m_animSunLight->set3Float("cameraPosition", cameraPos.x, cameraPos.y, cameraPos.z);
    m_animSunLight->set1Float("ambientLight", ambientLight);
    m_animSunLight->set1Float("specularLightStrength", specularLightStrength);

    std::string boneMatrixNameInShader;
    for(const std::shared_ptr<Beryll::BaseAnimatedObject>& ao : m_allAnimatedObjects)
    {
        if(ao->getIsEnabledOnScene())
        {
            MVP = Beryll::Camera::getPerspectiveView() * ao->getModelMatrix();
            m_animSunLight->setMatrix4x4Float("MVP_matrix", MVP);
            m_animSunLight->setMatrix4x4Float("model_matrix", ao->getModelMatrix());

            uint32_t boneCount = ao->getBoneCount();
            for(int i = 0; i < boneCount; ++i)
            {
                boneMatrixNameInShader = "bonesMatrices[";
                boneMatrixNameInShader += std::to_string(i);
                boneMatrixNameInShader += "]";
                m_animSunLight->setMatrix4x4Float(boneMatrixNameInShader.c_str(), ao->getBoneMatrices()[i].finalWorldTransform);
            }
            ao->useInternalShader = false;
            ao->draw();
        }
    }

    m_simpleSunLight->bind();
    m_simpleSunLight->set3Float("sunLightDir", sunLightDir.x, sunLightDir.y, sunLightDir.z);
    m_simpleSunLight->set3Float("cameraPosition", cameraPos.x, cameraPos.y, cameraPos.z);
    m_simpleSunLight->set1Float("ambientLight", ambientLight);
    m_simpleSunLight->set1Float("specularLightStrength", specularLightStrength);

    for(const std::shared_ptr<Beryll::BaseSimpleObject>& so : m_allSimpleObjects)
    {
        if(so->getIsEnabledOnScene())
        {
            MVP = Beryll::Camera::getPerspectiveView() * so->getModelMatrix();
            m_simpleSunLight->setMatrix4x4Float("MVP_matrix", MVP);
            m_simpleSunLight->setMatrix4x4Float("model_matrix", so->getModelMatrix());
            normalMatrix = glm::mat3(so->getModelMatrix());
            m_simpleSunLight->setMatrix3x3Float("normal_matrix", normalMatrix);
            so->useInternalShader = false;
            so->draw();
        }
    }

    // draw skybox last
    m_skyBox->draw();
}

void Play3DSceneLayer::playSound()
{
    for(const std::shared_ptr<Beryll::SceneObject>& so : m_sceneObjects)
    {
        if(so->getIsEnabledOnScene())
        {
            so->playSound();
        }
    }
}

void Play3DSceneLayer::createShaders()
{
    m_simpleSunLight = Beryll::Renderer::createShader("shaders/GLES/SimpleSunLight.vert", "shaders/GLES/SimpleSunLight.frag");
    m_simpleSunLight->activateDiffuseTexture();
    m_simpleSunLight->activateSpecularTexture();

    m_animSunLight = Beryll::Renderer::createShader("shaders/GLES/AnimationSunLight.vert", "shaders/GLES/AnimationSunLight.frag");
    m_animSunLight->activateDiffuseTexture();
    m_animSunLight->activateSpecularTexture();

    m_shadowMapSimple = Beryll::Renderer::createShader("shaders/GLES/shadowMap/Simple.vert", "shaders/GLES/Simple.frag");
    m_shadowMapAnim = Beryll::Renderer::createShader("shaders/GLES/shadowMap/Animation.vert", "shaders/GLES/Animation.frag");
}