#include "Mariokart.h"
#include "Events/GameEvents.h"
#include "DebugDraw/MyDebugDraw.h"
//#include "GameplayComponents/GameComponents.h"
#include "Objects/Controller.h"
#include "Game.h"


Mariokart::Mariokart(fw::GameCore* pGameCore, Controller* pController) : Scene(pGameCore, pController)
{
    m_ComponentManager = new fw::ComponentManager();

    m_pResourceManager = m_pGameCore->GetResourceManager();

    // Create a physics world.
    b2Vec2 gravity(0, 0);
    m_pPhysicsWorld = new b2World(gravity);

    //Debug Draw Initialized
    m_pDebugDraw = new MyDebugDraw();
    m_pDebugDraw->SetFlags(b2Draw::e_shapeBit);
    m_pDebugDraw->m_pUniforms = m_pGameCore->GetUniforms();
    m_pDebugDraw->m_ShaderHandle = m_pResourceManager->GetShader("VertexColor");
    m_pDebugDraw->m_ViewID = 1;
    m_pDebugDraw->m_IsXY = false;
    m_pPhysicsWorld->SetDebugDraw(m_pDebugDraw);

    //Create Contact Listener.
    m_pContactListener = new fw::MyContactListener(m_pGameCore->GetEventManager());
    m_pPhysicsWorld->SetContactListener(m_pContactListener);


    //Track Creation
    m_Objects.push_back(new fw::GameObject(this, "Track"));
    fw::TransformComponent* platformTransform = new fw::TransformComponent(m_Objects[0], vec3(75, 0, 75), vec3(-90, 0, 0), vec3(150, 150, 1));
    fw::RenderComponent* platformRender = new fw::RenderComponent(m_Objects[0], m_pResourceManager->GetMesh("Sprite"), m_pResourceManager->GetMaterial("Track"));
    
    //Enables all GameObject components
    for (auto& i : m_Objects)
    {
        i->Enable(m_ComponentManager);
    }
    

    //Kart creation
    CreateKart(vec3(10, 0.5f, 10), "Mario");
    CreateKart(vec3(12, 0.5f, 10), "Toad");
    CreateKart(vec3(14, 0.5f, 10), "Bowser");
    CreateKart(vec3(16, 0.5f, 10), "Luigi");

    //Set Active Kart and attach controller
    m_pActiveKart = m_pKarts[0];
    m_pActiveKart->GetComponent<KartLogicComponent>()->SetController(m_pController);
    
    //Set active Camera
    m_pActiveCamera = new fw::CameraComponent(m_pKarts[0], vec3(0, 5, -10));

    for (fw::GameObject* i : m_pKarts)
    {
        i->Enable(m_ComponentManager);
    }
    
    //Track Collision Creation
    BuildTrackCollision();

    for (auto& i : m_pTrackCollision)
    {
        i->Enable(m_ComponentManager);
    }


    //Sensor Object Creation (I am experimenting with std::pair and std::tuple, I see it wasn't really needed here, but I wanted to practice it)
    std::pair<SensorComponent::SensorType, vec3> sensorObjects[] = {

        {SensorComponent::SensorType::Coin, vec3(12.25f, 0.5f, 102.6f)},
        {SensorComponent::SensorType::Coin, vec3(18.1f, 0.5f, 134.2f)},
        {SensorComponent::SensorType::Coin, vec3(20.5f, 0.5f, 132)},
        {SensorComponent::SensorType::Coin, vec3(30.95f, 0.5f, 138.868f)},
        {SensorComponent::SensorType::OilSlick, vec3(117, 0.5f, 142)},
        {SensorComponent::SensorType::OilSlick, vec3(79.7f, 0.5f, 33.1f)},
        {SensorComponent::SensorType::SpeedPad, vec3(14, 0.5f, 35.8f)}
    };

    for (auto& i : sensorObjects)
    {
        CreateSensorObject(i.first, i.second);
    }


    for (auto& i : m_pSensorObjects)
    {
        i->Enable(m_ComponentManager);
    }

    //Coin Object Creation
    m_pCoinObject = new fw::GameObject(this, "Player Coin");
    fw::TransformComponent* coinTransform = new fw::TransformComponent(m_pCoinObject, vec3(75, 0, 75), vec3(0, 0, 0), vec3(1, 1, 1));
    fw::RenderComponent* coinRender = new fw::RenderComponent(m_pCoinObject, m_pResourceManager->GetMesh("Kart"), m_pResourceManager->GetMaterial("Coin"));

    m_pCoinObject->Enable(m_ComponentManager);
    m_pCoinParent = m_pKarts[0];

    //Removing from render on Startup so it is only visible when its trggered.
    m_ComponentManager->RemoveComponent(m_pCoinObject->GetComponent<fw::RenderComponent>());

    //Initially syncs the transform and all the physics
    m_ComponentManager->SyncPhysicsToTransform(false);

    //Ignore this part. I am keeping this here so I know how to do it later
    {
        //Joint Creation
        //b2RevoluteJointDef jointDef;
        ////b2Vec2 anchorPos(6, 1);
        ////jointDef.Initialize(m_Objects[2]->GetBody(), m_pPlayer->GetBody(), anchorPos);

        //jointDef.bodyA = m_Objects[1]->GetComponent<fw::PhysicsComponent>()->GetBody();
        //jointDef.bodyB = m_Objects[2]->GetComponent<fw::PhysicsComponent>()->GetBody();
        //jointDef.localAnchorA = b2Vec2(0, 0);
        //jointDef.localAnchorB = b2Vec2(0, -5);
        //jointDef.collideConnected = false;
        //jointDef.motorSpeed = 2000.0f;
        //jointDef.enableMotor = true;

        //m_pPhysicsWorld->CreateJoint(&jointDef);
    }
}

////////////////////////////////////////////////////////
// Destructor

Mariokart::~Mariokart()
{
    delete m_pDebugDraw;

    for (fw::GameObject* pObject : m_pTrackCollision)
    {
        delete pObject;
    }

    for (fw::GameObject* pObject : m_pKarts)
    {
        delete pObject;
    }

    for (fw::GameObject* pObject : m_pSensorObjects)
    {
        delete pObject;
    }
    
    delete m_pCoinObject;

}

////////////////////////////////////////////////////////
// CreateKart

void Mariokart::CreateKart(vec3 position, const char* MaterialName)
{
    fw::GameObject* newKart = new fw::GameObject(this, MaterialName);

    fw::TransformComponent* kartTransform = new fw::TransformComponent(newKart, position, vec3(0, 0, 0), vec3(1, 1, 1));
    fw::RenderComponent* kartRender = new fw::RenderComponent(newKart, m_pResourceManager->GetMesh("OBJTest"), m_pResourceManager->GetMaterial("Car"));
    fw::PhysicsComponent* kartPhysics = new fw::PhysicsComponent(newKart);
    newKart->GetComponent<fw::PhysicsComponent>()->CreateBody(m_pPhysicsWorld, b2_dynamicBody, false);
    KartLogicComponent* kartLogic = new KartLogicComponent(newKart);

    m_pKarts.push_back(newKart);
}

////////////////////////////////////////////////////////
// ChangeActiveKart

void Mariokart::ChangeActiveKart(fw::GameObject* kart)
{
    m_pActiveKart->GetComponent<KartLogicComponent>()->SetController(nullptr);
    m_pActiveKart->RemoveComponent(m_pActiveCamera);
    m_pActiveKart = kart;
    m_pActiveKart->GetComponent<KartLogicComponent>()->SetController(m_pController);
    m_pActiveKart->AddComponents(m_pActiveCamera);
    m_pActiveCamera->SetGameObject(kart);
}

////////////////////////////////////////////////////////
// BuildTrackCollision

void Mariokart::BuildTrackCollision()
{
    std::tuple<vec3, vec3, vec3> values[] =
    {
        { vec3(35, 0, 20.0f), vec3(0, 0, 0), vec3(1.0f, 4.5f, 0) },
        { vec3(30.5f, 0, 84), vec3(0, 0, 0), vec3(9.5f, 42.25f, 0) },
        { vec3(37, 0, 33.25f), vec3(0, 0, 0), vec3(10.5f, 9, 0) },
        { vec3(102.1f, 0, 97.5f), vec3(0, 0, 0), vec3(3, 3, 0) },
        { vec3(60, 0, 90.85f), vec3(0, 0, 0), vec3(20, 3, 0) },
        { vec3(89.7f, 0, 91.5f), vec3(0, 0, 0), vec3(10, 2, 0) },
        { vec3(116, 0, 109.5f), vec3(0, 0, 0), vec3(10, 15, 0) },
        { vec3(75, 0, 133.5f), vec3(0, 0, 0), vec3(9, 17, 0) },
        { vec3(75.6f, 0, 39.4f), vec3(0, 0, 0), vec3(40, 3, 0) },
        { vec3(115, 0, 35.3f), vec3(0, 0, 0), vec3(12, 7, 0) },
        { vec3(136, 0, 73.6f), vec3(0, 0, 0), vec3(14, 4, 0) },
        { vec3(108.3f, 0, 71.3f), vec3(0, 0, 0), vec3(14, 2, 0) },
        { vec3(83.8f, 0, 66.6f), vec3(0, 0, 0), vec3(12, 2, 0) },
        { vec3(76, 0, 9.7f), vec3(0, 0, 0), vec3(1, 11, 0) },
        { vec3(75, 0, -0.5f), vec3(0, 0, 0), vec3(75, 1, 0) },
        { vec3(75, 0, 150.5f), vec3(0, 0, 0), vec3(75, 1, 0) },
        { vec3(-0.5f, 0, 75), vec3(0, 0, 0), vec3(1, 75, 0) },
        { vec3(150.5f, 0, 75), vec3(0, 0, 0), vec3(1, 75, 0) }
    };

    for (auto& i : values)
    {
        fw::GameObject* newTrackCollision = new fw::GameObject(this, "Wall");
        fw::TransformComponent* wallTransform = new fw::TransformComponent(newTrackCollision, std::get<0>(i), std::get<1>(i), std::get<2>(i));
        fw::PhysicsComponent* wallPhysics = new fw::PhysicsComponent(newTrackCollision);
        newTrackCollision->GetComponent<fw::PhysicsComponent>()->CreateBody(m_pPhysicsWorld, b2_staticBody, false, true);
        m_pTrackCollision.push_back(newTrackCollision);
    }
}

////////////////////////////////////////////////////////
// HandleSensorCollision

void Mariokart::HandleSensorCollision(fw::GameObject* pObjectSensor, fw::GameObject* pObjectKart)
{
    SensorComponent* pSensor = pObjectSensor->GetComponent<SensorComponent>();
    KartLogicComponent* pKart = pObjectKart->GetComponent<KartLogicComponent>();

    if (pSensor->GetIsCoin())
    {
        pKart->SetNumCoins(pKart->GetNumCoins() + 1);
        pKart->SetHitCoin(true);

        m_ComponentManager->AddComponent(m_pCoinObject->GetComponent<fw::RenderComponent>());
        m_pCoinParent = pObjectKart;
    }
    else if (pSensor->GetIsOilSlick())
    {
        pKart->SetHitOilSlick(true);
        pKart->SetNumCoins(0);
    }
    else if (pSensor->GetIsSpeedPad())
    {
        float PadAngle = pObjectSensor->GetComponent<fw::PhysicsComponent>()->GetBody()->GetAngle();
        b2Vec2 direction = b2Vec2(cos(PadAngle + fw::degreesToRads(90)), sin(PadAngle + fw::degreesToRads(90)));
        pKart->SetHitSpeedPad(true, direction);
    }

}

////////////////////////////////////////////////////////
// Create Sensor Object

void Mariokart::CreateSensorObject(SensorComponent::SensorType type, vec3 position)
{
    fw::GameObject* pNewSensorObject = new fw::GameObject(this, "Sensor");

    if (type == SensorComponent::SensorType::Coin)
    {
        pNewSensorObject->SetName("Coin");
        fw::TransformComponent* CoinTransform = new fw::TransformComponent(pNewSensorObject, position, vec3(0, 0, 0), vec3(1, 1, 1));
        SensorComponent* CoinSensor = new SensorComponent(pNewSensorObject, true);
    }
    else if (type == SensorComponent::SensorType::OilSlick)
    {
        pNewSensorObject->SetName("OilSlick");
        fw::TransformComponent* OilSlickTransform = new fw::TransformComponent(pNewSensorObject, position, vec3(0, 0, 0), vec3(2, 2, 1));
        SensorComponent* OilSlickSensor = new SensorComponent(pNewSensorObject, false, true);
    }
    else if (type == SensorComponent::SensorType::SpeedPad)
    {
        pNewSensorObject->SetName("SpeedPad");
        fw::TransformComponent* SpeedPadTransform = new fw::TransformComponent(pNewSensorObject, position, vec3(0, 0, 0), vec3(2, 2, 1));
        SensorComponent* SpeedPadSensor = new SensorComponent(pNewSensorObject, false, false, true);
    }

    fw::PhysicsComponent* CoinPhysics = new fw::PhysicsComponent(pNewSensorObject);
    pNewSensorObject->GetComponent<fw::PhysicsComponent>()->CreateBody(m_pPhysicsWorld, b2_staticBody, true);
    m_pSensorObjects.push_back(pNewSensorObject);
}

////////////////////////////////////////////////////////
// StartFrame

void Mariokart::StartFrame(float deltaTime)
{

}

////////////////////////////////////////////////////////
// OnEvent

void Mariokart::OnEvent(fw::Event* pEvent)
{
    // Process Collision Events.
    if (pEvent->GetType() == fw::CollisionEvent::GetStaticEventType())
    {
        fw::CollisionEvent* pCollisonEvent = static_cast<fw::CollisionEvent*>(pEvent);

        //pCollisonEvent->GetObjectA()->OnCollide(pCollisonEvent->GetObjectB());

        if (pCollisonEvent->GetObjectB()->GetComponent<fw::PhysicsComponent>()->GetBody()->GetFixtureList()->IsSensor() == true)
        {
            HandleSensorCollision(pCollisonEvent->GetObjectB(), pCollisonEvent->GetObjectA());
        }
        else if (pCollisonEvent->GetObjectA()->GetComponent<fw::PhysicsComponent>()->GetBody()->GetFixtureList()->IsSensor() == true)
        {
            HandleSensorCollision(pCollisonEvent->GetObjectA(), pCollisonEvent->GetObjectB());
        }
    }

    // Remove object from GameObject list and delete it.
    if (pEvent->GetType() == RemoveFromGameEvent::GetStaticEventType())
    {
        RemoveFromGameEvent* pRemoveFromGameEvent = static_cast<RemoveFromGameEvent*>(pEvent);
        fw::GameObject* pObject = pRemoveFromGameEvent->GetGameObject();

        auto it = std::find( m_Objects.begin(), m_Objects.end(), pObject );
        m_Objects.erase( it );

        delete pObject;
    }

    // Set the new aspect ratio in the camera.
    if (pEvent->GetType() == fw::WindowResizeEvent::GetStaticEventType())
    {
        int width = m_pGameCore->GetFramework()->GetWindowClientWidth();
        int height = m_pGameCore->GetFramework()->GetWindowClientHeight();

        m_pActiveCamera->SetAspectRatio( (float)width/height );
        
    }
}

////////////////////////////////////////////////////////
// Update

void Mariokart::Update(float deltaTime)
{
    if (m_pCoinParent->GetComponent<KartLogicComponent>()->GetHitCoin())
    {
        m_pCoinObject->GetComponent<fw::TransformComponent>()->SetPosition(m_pCoinParent->GetComponent<fw::TransformComponent>()->GetPosition() + vec3(0, 1, 0));
        m_pCoinObject->GetComponent<fw::TransformComponent>()->SetRotation(m_pCoinObject->GetComponent<fw::TransformComponent>()->GetRotation() + vec3(0, 10, 0));
    }

    for (fw::GameObject* pObject : m_Objects)
    {
        pObject->Update(deltaTime);
    }

    for (fw::GameObject* pObject : m_pTrackCollision)
    {
        pObject->Update(deltaTime);
    }

    std::vector<fw::Component*> kartComponents = m_ComponentManager->GetComponentVector<KartLogicComponent>();

    for (auto& i : kartComponents)
    {
        KartLogicComponent* Kart = static_cast<KartLogicComponent*>(i);

        Kart->UpdateKart();
    }

    if (static_cast<Game*>(m_pGameCore)->GetGameState() == GameState::Play)
    {
        m_TimeSinceLastPhysicsStep += deltaTime;

        while (m_TimeSinceLastPhysicsStep >= 1 / 60.0f)
        {
            m_pPhysicsWorld->Step(1 / 60.0f, 8, 3);
            m_TimeSinceLastPhysicsStep -= 1 / 60.0f;
        }
        m_ComponentManager->SyncPhysics(false);
    }
    else
    {
        m_ComponentManager->SyncPhysicsToTransform(false);
    }

    //Swap Kart
    if (m_pController->WasPressed(Controller::Mask::Action))
    {
        if (m_pActiveKart == m_pKarts[0])
        {
            ChangeActiveKart(m_pKarts[1]);
        }
        else if (m_pActiveKart == m_pKarts[1])
        {
            ChangeActiveKart(m_pKarts[2]);
        }
        else if (m_pActiveKart == m_pKarts[2])
        {
            ChangeActiveKart(m_pKarts[3]);
        }
        else
        {
            ChangeActiveKart(m_pKarts[0]);
        }
    }

}

////////////////////////////////////////////////////////
// Draw

void Mariokart::Draw(int viewID)
{
    
    m_ComponentManager->UpdateCamera();

    m_ComponentManager->UpdateTransforms();
    m_ComponentManager->RenderAll(viewID);

    m_pPhysicsWorld->DebugDraw();

}

////////////////////////////////////////////////////////
// Display Object List

void Mariokart::Editor_DisplayObjectList()
{

    ImGui::Begin("Object List");

    Game* pGame = static_cast<Game*>(m_pGameCore);

    for (fw::GameObject* pObject : m_Objects)
    {
        bool isSelected = (pObject == pGame->GetSelectedGameObject());
        if (ImGui::Selectable(pObject->GetName().c_str(), &isSelected))
        {
            pGame->SetIsSelected(pObject);
        }
    }

    for (fw::GameObject* pObject : m_pKarts)
    {
        bool isSelected = (pObject == pGame->GetSelectedGameObject());
        if (ImGui::Selectable(pObject->GetName().c_str(), &isSelected))
        {
            pGame->SetIsSelected(pObject);
        }
    }

    for (fw::GameObject* pObject : m_pSensorObjects)
    {
        bool isSelected = (pObject == pGame->GetSelectedGameObject());
        if (ImGui::Selectable(pObject->GetName().c_str(), &isSelected))
        {
            pGame->SetIsSelected(pObject);
        }
    }
    ImGui::End(); // "Object List"
}