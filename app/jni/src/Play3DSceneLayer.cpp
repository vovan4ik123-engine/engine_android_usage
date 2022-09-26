#include "Play3DSceneLayer.h"

Play3DSceneLayer::Play3DSceneLayer(std::shared_ptr<PlayGUILayer> guiLayer)
{
    m_ID = Beryll::LayerID::PLAY_SCENE;

    m_guiLayer = std::move(guiLayer);

//    simpleCubeSphere = std::make_shared<Beryll::SimpleObject>("models/cube_text.dae",
//                                                              "shaders/GLES/SimpleModel.vert",
//                                                              "shaders/GLES/SimpleModel.frag",
//                                                              "diffuseTexture");
//    m_gameObjects.push_back(simpleCubeSphere);
//
//    enemyMan = std::make_shared<Beryll::AnimatedObject>("models/garbage/model.dae",
//                                                        "shaders/GLES/AnimatedModel.vert",
//                                                        "shaders/GLES/AnimatedModel.frag",
//                                                        "diffuseTexture");
//    m_gameObjects.push_back(enemyMan);

    for(int i = 0; i < 500; ++i)
    {
        std::shared_ptr<Beryll::AnimatedObject> animatedObject =
                        std::make_shared<Beryll::AnimatedObject>("models/garbage/model.dae",
                                                                "shaders/GLES/AnimatedModel.vert",
                                                                "shaders/GLES/AnimatedModel.frag",
                                                                "diffuseTexture");

        m_gameObjects.push_back(animatedObject);
        animatedObject->setOrigin(glm::vec3(Beryll::RandomGenerator::getFastInt(0, 90),
                                              10.0f,
                                              -Beryll::RandomGenerator::getFastInt(0, 90)));
        BR_INFO("animated origin %f %f %f", animatedObject->getOrigin().x, animatedObject->getOrigin().y, animatedObject->getOrigin().z);
    }
    for(int i = 0; i < 0; ++i)
    {
        std::shared_ptr<Beryll::CollidingSimpleObject> testBall
                          = std::make_shared<Beryll::CollidingSimpleObject>("models/garbage/SphereSphere.dae",
                                                                            5.0f,
                                                                            true,
                                                                            Beryll::CollisionFlags::DYNAMIC,
                                                                            Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT,
                                                                            Beryll::CollisionGroups::STATIC_ENVIRONMENT | Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT | Beryll::CollisionGroups::PLAYER,
                                                                            "shaders/GLES/SimpleModel.vert",
                                                                            "shaders/GLES/SimpleModel.frag",
                                                                            "diffuseTexture");

        m_gameObjects.push_back(testBall);
        testBall->setOrigin(glm::vec3(Beryll::RandomGenerator::getFastInt(0, 90),
                                      15.0f,
                                      -Beryll::RandomGenerator::getFastInt(0, 90)));
    }
//
//    for(int i = 0; i < 10; ++i)
//    {
//        collWarms.push_back(std::make_shared<Beryll::CollidingAnimatedObject>("models/collisionsAnim.dae",
//                                                                              15,
//                                                                              false,
//                                                                              Beryll::CollisionFlags::DYNAMIC,
//                                                                              Beryll::CollisionGroups::CUBE,
//                                                                              Beryll::CollisionGroups::STATIC_ENVIRONMENT | Beryll::CollisionGroups::WALL |
//                                                                              Beryll::CollisionGroups::CUBE | Beryll::CollisionGroups::PLAYER,
//                                                                              "shaders/GLES/AnimatedModel.vert",
//                                                                              "shaders/GLES/AnimatedModel.frag",
//                                                                              "diffuseTexture"));
//        m_gameObjects.push_back(collWarms.back());
//    }

//                                                                              SimCollGround001_0kg

    std::string modelName;
    for(int i = 1; i <= 1; ++i)
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
            modelName = "models/TestWorld/";
            modelName += std::to_string(i);
            modelName += ".dae";
        }
        std::shared_ptr<Beryll::CollidingSimpleObject> testWorld
                  = std::make_shared<Beryll::CollidingSimpleObject>(modelName.c_str(),
                                                                    0,
                                                                    true,
                                                                    Beryll::CollisionFlags::STATIC,
                                                                    Beryll::CollisionGroups::STATIC_ENVIRONMENT | Beryll::CollisionGroups::GROUND,
                                                                    Beryll::CollisionGroups::PLAYER | Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT | Beryll::CollisionGroups::CAMERA,
                                                                    "shaders/GLES/SimpleModel.vert",
                                                                    "shaders/GLES/SimpleModel.frag",
                                                                    "diffuseTexture");
        m_gameObjects.push_back(testWorld);
    }
//    collPlane = std::make_shared<Beryll::CollidingSimpleObject>("models/garbage/SimCollGround001_0kg.dae",
//                                                                0,
//                                                                true,
//                                                                Beryll::CollisionFlags::STATIC,
//                                                                Beryll::CollisionGroups::STATIC_ENVIRONMENT,
//                                                                Beryll::CollisionGroups::PLAYER | Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT | Beryll::CollisionGroups::CAMERA,
//                                                                "shaders/GLES/SimpleModel.vert",
//                                                                "shaders/GLES/SimpleModel.frag",
//                                                                "diffuseTexture");
//    m_gameObjects.push_back(collPlane);

    player = std::make_shared<Beryll::CollidingSimplePlayer>("models/garbage/CapsuleCapsule.dae",
                                                             10.0f,
                                                             true,
                                                             Beryll::CollisionFlags::DYNAMIC,
                                                             Beryll::CollisionGroups::PLAYER,
                                                             Beryll::CollisionGroups::STATIC_ENVIRONMENT | Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT,
                                                             "shaders/GLES/SimpleModel.vert",
                                                             "shaders/GLES/SimpleModel.frag",
                                                             "diffuseTexture");

    m_gameObjects.push_back(player);

    player->setOrigin(glm::vec3(0.0f, 20.0f,0.0f));

    Beryll::Camera::setCameraPos(player->getOrigin() + glm::vec3(50.0f, 0.0f, 0.0f));
    Beryll::Camera::setCameraFront(player->getOrigin());
}

Play3DSceneLayer::~Play3DSceneLayer()
{

}

void Play3DSceneLayer::updateBeforePhysics()
{
    // 1. react on user input

    if(m_guiLayer->buttonMove->getIsPressed()) // repeat action
    {
        player->move(Beryll::MoveDirection::FORWARD);
    }

    if(m_guiLayer->buttonResetCube->getIsPressed()) // one action
    {
        player->jump();
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
            m_cameraOffset.x = (glm::cos(glm::radians(m_angleXZ)) * glm::cos(glm::radians(m_angleYZ)))  * 100;
            m_cameraOffset.y = glm::sin(glm::radians(m_angleYZ))  * 100;
            m_cameraOffset.z = (glm::sin(glm::radians(m_angleXZ)) * glm::cos(glm::radians(m_angleYZ)))  * 100;

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

    std::function<void(const std::vector<std::shared_ptr<Beryll::GameObject>>&, int, int)> updateBeforePhysics =
            [this](const std::vector<std::shared_ptr<Beryll::GameObject>>& v, int begin, int end) -> void // -> void = return type
            {
                for(int i = begin; i < end; ++i)
                {
                    if(Beryll::Physics::getIsCollisionGroupContainsOther(v[i]->getCollisionGroup(),Beryll::CollisionGroups::GROUND))
                    {
                        if(glm::distance(player->getOrigin(), v[i]->getOrigin()) > 200.0f)
                        {
                            v[i]->disable();
                        }
                        else
                        {
                            v[i]->enable();
                        }
                    }
                    else if(Beryll::Physics::getIsCollisionGroupContainsOther(v[i]->getCollisionGroup(),Beryll::CollisionGroups::PLAYER))
                    {
                        // dont disable player
                    }
                    else
                    {
                        if(Beryll::Camera::getIsSeeObject(v[i]->getOrigin()))
                        {
                            v[i]->enable();
                        }
                        else
                        {
                            v[i]->disable();
                        }
                    }

                    if(v[i]->getIsEnabled())
                    {
                        v[i]->updateBeforePhysics();
                    }
                }
            };

    Beryll::AsyncRun::Run(m_gameObjects, updateBeforePhysics);
}

void Play3DSceneLayer::updateAfterPhysics()
{
    // 1. let objects update themselves based on physics simulation
    std::function<void(const std::vector<std::shared_ptr<Beryll::GameObject>>&, int, int)> updateAfterPhysics =
            [](const std::vector<std::shared_ptr<Beryll::GameObject>>& v, int begin, int end) -> void // -> void = return type
            {
                for(int i = begin; i < end; ++i)
                {
                    if(v[i]->getIsEnabled())
                    {
                        v[i]->updateAfterPhysics();
                    }
                }
            };

    Beryll::AsyncRun::Run(m_gameObjects, updateAfterPhysics);

    int numActive = 0;
    for(const std::shared_ptr<Beryll::GameObject>& go : m_gameObjects)
    {
        if(go->getIsEnabled())
        {
            ++numActive;
        }
    }
    BR_INFO("Active number:%d", numActive);

    // 2. resolve collisions
    //if(Beryll::Physics::getIsCollision(collWall->getID(), player->getID()))
    //    BR_INFO("Player collising with wall");
    //if(Beryll::Physics::getIsCollision(collPlane->getID(), player->getID()))
    //   BR_INFO("Player collising with ground");

    // 3. use objects updated position
//    Beryll::RayClosestHit rayClosestHit = Beryll::Physics::castRayClosestHit(player->getOrigin(),
//                                                                          player->getOrigin() + m_cameraOffset,
//                                                                          Beryll::CollisionGroups::CAMERA,
//                                                                          Beryll::CollisionGroups::STATIC_ENVIRONMENT | Beryll::CollisionGroups::WALL);
//    if(rayClosestHit)
//        Beryll::Camera::setCameraPos(rayClosestHit.hitPoint);
//    else
//        Beryll::Camera::setCameraPos(player->getOrigin() + m_cameraOffset);
//
//  Beryll::Camera::setCameraFront(player->getOrigin());

    Beryll::Camera::setCameraPos(player->getOrigin() + m_cameraOffset);
    Beryll::Camera::setCameraFront(player->getOrigin());
}

void Play3DSceneLayer::draw()
{
    for(const std::shared_ptr<Beryll::GameObject>& go : m_gameObjects)
    {
        if(go->getIsEnabled())
        {
            go->draw(); // DONT call in parallel
        }
    }
}

void Play3DSceneLayer::playSound()
{
    for(const std::shared_ptr<Beryll::GameObject>& go : m_gameObjects)
    {
        if(go->getIsEnabled())
        {
            go->playSound();
        }
    }
}