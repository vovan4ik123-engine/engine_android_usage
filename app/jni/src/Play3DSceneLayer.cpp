#include "Play3DSceneLayer.h"

Play3DSceneLayer::Play3DSceneLayer(std::shared_ptr<PlayGUILayer> guiLayer)
{
    m_ID = Beryll::LayerID::PLAY_SCENE;

    m_guiLayer = std::move(guiLayer);

//    simpleCubeSphere = std::make_shared<Beryll::SimpleObject>("models/cube_text.dae",
//                                                              false,
//                                                              "shaders/GLES/SimpleModel.vert",
//                                                              "shaders/GLES/SimpleModel.frag",
//                                                              "diffuseTexture");
//    m_gameObjects.push_back(simpleCubeSphere);
//
    enemyMan = std::make_shared<Beryll::AnimatedObject>("models/garbage/model.dae",
                                                        true,
                                                        "shaders/GLES/AnimatedModel.vert",
                                                        "shaders/GLES/AnimatedModel.frag",
                                                        "diffuseTexture");
    m_gameObjects.push_back(enemyMan);

    collPlane = std::make_shared<Beryll::CollidingSimpleObject>("models/garbage/CollisionStaticGround.dae",
                                                                false,
                                                                0,
                                                                true,
                                                                Beryll::CollisionFlags::STATIC,
                                                                Beryll::CollisionGroups::STATIC_ENVIRONMENT,
                                                                Beryll::CollisionGroups::PLAYER | Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT | Beryll::CollisionGroups::CAMERA,
                                                                "shaders/GLES/SimpleModel.vert",
                                                                "shaders/GLES/SimpleModel.frag",
                                                                "diffuseTexture");
    m_gameObjects.push_back(collPlane);

    for(int i = 0; i < 0; ++i)
    {
        collCubes.push_back(std::make_shared<Beryll::CollidingSimpleObject>("models/garbage/CubeCapsule.dae",
                                                                            true,
                                                                            1.0f,
                                                                            true,
                                                                            Beryll::CollisionFlags::DYNAMIC,
                                                                            Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT,
                                                                            Beryll::CollisionGroups::STATIC_ENVIRONMENT | Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT | Beryll::CollisionGroups::PLAYER,
                                                                            "shaders/GLES/SimpleModel.vert",
                                                                            "shaders/GLES/SimpleModel.frag",
                                                                            "diffuseTexture"));

        m_gameObjects.push_back(collCubes.back());
        collCubes.back()->setOrigin(glm::vec3(Beryll::RandomGenerator::getFastInt(50, 150),
                                                  100.0f,
                                                  Beryll::RandomGenerator::getFastInt(-50, 50)));
    }
//
//    for(int i = 0; i < 10; ++i)
//    {
//        collWarms.push_back(std::make_shared<Beryll::CollidingAnimatedObject>("models/collisionsAnim.dae",
//                                                                              false,
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
//
//    Beryll::Physics::setAngularFactor(player->getID(), glm::vec3(0.0f));
//    Beryll::Physics::disableGravityForObject(player->getID());

    player = std::make_shared<Beryll::CollidingSimplePlayer>("models/garbage/CapsuleCapsule.dae",
                                                             false,
                                                             3.0f,
                                                             true,
                                                             Beryll::CollisionFlags::DYNAMIC,
                                                             Beryll::CollisionGroups::PLAYER,
                                                             Beryll::CollisionGroups::STATIC_ENVIRONMENT | Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT,
                                                             "shaders/GLES/SimpleModel.vert",
                                                             "shaders/GLES/SimpleModel.frag",
                                                             "diffuseTexture");

    m_gameObjects.push_back(player);

    player->setOrigin(glm::vec3(120.0f, 100.0f,0.0f));

    Beryll::Camera::setCameraPos(player->getOrigin() + glm::vec3(50.0f, 0.0f, 0.0f));
    Beryll::Camera::setCameraFront(player->getOrigin());
//
//    Beryll::PhysicsTransforms t = Beryll::Physics::getTransforms(testCube->getID());
//    BR_INFO("Cube origin: x:{0}, y:{1}, z:{2}", t.position.x, t.position.y, t.position.z);
//
//    ground1 = std::make_shared<Beryll::CollidingSimpleObject>("models/scene1/ground1.dae",
//                                                               false,
//                                                               0.0f,
//                                                               false,
//                                                               Beryll::CollisionFlags::STATIC,
//                                                               Beryll::CollisionGroups::STATIC_ENVIRONMENT,
//                                                               Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT,
//                                                               "shaders/GLES/SimpleModel.vert",
//                                                               "shaders/GLES/SimpleModel.frag",
//                                                               "diffuseTexture");
//
//    m_gameObjects.push_back(ground1);
//
//    ground2 = std::make_shared<Beryll::CollidingSimpleObject>("models/scene1/ground2.dae",
//                                                               false,
//                                                               0.0f,
//                                                               false,
//                                                               Beryll::CollisionFlags::STATIC,
//                                                               Beryll::CollisionGroups::STATIC_ENVIRONMENT,
//                                                               Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT,
//                                                               "shaders/GLES/SimpleModel.vert",
//                                                               "shaders/GLES/SimpleModel.frag",
//                                                               "diffuseTexture");
//
//    m_gameObjects.push_back(ground2);
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
            m_cameraOffset.x = (glm::cos(glm::radians(m_angleXZ)) * glm::cos(glm::radians(m_angleYZ)))  * 50;
            m_cameraOffset.y = glm::sin(glm::radians(m_angleYZ))  * 50;
            m_cameraOffset.z = (glm::sin(glm::radians(m_angleXZ)) * glm::cos(glm::radians(m_angleYZ)))  * 50;

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
            [pl = player](const std::vector<std::shared_ptr<Beryll::GameObject>>& v, int begin, int end) -> void // -> void = return type
            {
                for(int i = begin; i < end; ++i)
                {
                    bool see = Beryll::Camera::getIsSeeObject(v[i]->getOrigin());
                    pl->getOrigin();

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
    for(const std::shared_ptr<Beryll::GameObject>& go : m_gameObjects)
    {
        if(go->getIsEnabled())
        {
            go->updateAfterPhysics();
        }
    }

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
            go->draw();
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