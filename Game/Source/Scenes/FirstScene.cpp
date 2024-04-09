#include "FirstScene.h"
#include "Events/GameEvents.h"
#include "DebugDraw/MyDebugDraw.h"
#include "GameplayComponents/GameComponents.h"
#include "Game.h"


FirstScene::FirstScene(fw::GameCore* pGameCore, Controller* pController) : Scene(pGameCore, pController)
{
    m_ComponentManager = new fw::ComponentManager();

    m_pResourceManager = m_pGameCore->GetResourceManager();

    // Create a physics world.
    b2Vec2 gravity(0, -10);
    m_pPhysicsWorld = new b2World(gravity);

    //Debug Draw Initialized
    m_pDebugDraw = new MyDebugDraw();
    m_pDebugDraw->SetFlags(b2Draw::e_shapeBit);
    m_pDebugDraw->m_pUniforms = m_pGameCore->GetUniforms();
    m_pDebugDraw->m_ShaderHandle = m_pResourceManager->GetShader("VertexColor");
    m_pDebugDraw->m_ViewID = EditorView_Box2D_DebugDraw;
    m_pPhysicsWorld->SetDebugDraw(m_pDebugDraw);

    //Create Contact Listener.
    m_pContactListener = new fw::MyContactListener(m_pGameCore->GetEventManager());
    m_pPhysicsWorld->SetContactListener(m_pContactListener);

    //Object Creation
    m_Objects.push_back(new fw::GameObject(this, "Player"));

    m_Objects.push_back(new fw::GameObject(this, "Object 1"));
    m_Objects.push_back(new fw::GameObject(this, "Die"));
    m_Objects.push_back(new fw::GameObject(this, "Camera"));

    //Player Components
    fw::TransformComponent* playerTransform = new fw::TransformComponent(m_Objects[0], vec3(6, 9, 0), vec3(0, 0, 0), vec3(1, 1, 1));
    fw::RenderComponent* playerRender = new fw::RenderComponent(m_Objects[0], m_pResourceManager->GetMesh("Kart"), m_pResourceManager->GetMaterial("Bowser"));
    fw::PhysicsComponent* playerPhysics = new fw::PhysicsComponent(m_Objects[0]);
    PlayerLogicComponent* playerLogic = new PlayerLogicComponent(m_Objects[0]);
    m_Objects[0]->GetComponent<fw::PhysicsComponent>()->CreateBody(m_pPhysicsWorld, b2_dynamicBody, false);
    fw::CameraComponent* cameraComp = new fw::CameraComponent(m_Objects[0], vec3(0, 0, -20));
    m_Objects[0]->GetComponent<PlayerLogicComponent>()->SetController(m_pController);
    m_pActiveCamera = cameraComp;

    //Platform Components
    fw::TransformComponent* platformTransform = new fw::TransformComponent(m_Objects[1], vec3(6, 2, 0), vec3(0, 0, 0), vec3(20, 1, 1));
    fw::RenderComponent* platformRender = new fw::RenderComponent(m_Objects[1], m_pResourceManager->GetMesh("Square"), m_pResourceManager->GetMaterial("VertexColor"));
    fw::PhysicsComponent* platformPhysics = new fw::PhysicsComponent(m_Objects[1]);
    m_Objects[1]->GetComponent<fw::PhysicsComponent>()->CreateBody(m_pPhysicsWorld, b2_staticBody, false);

    //Die Components
    fw::TransformComponent* cubeTransform = new fw::TransformComponent(m_Objects[2], vec3(8, 4, 0), vec3(0, 0, 0), vec3(1, 1, 1));
    fw::RenderComponent* cubeRender = new fw::RenderComponent(m_Objects[2], m_pResourceManager->GetMesh("Cube"), m_pResourceManager->GetMaterial("Die"));
    fw::PhysicsComponent* cubePhysics = new fw::PhysicsComponent(m_Objects[2]);
    m_Objects[2]->GetComponent<fw::PhysicsComponent>()->CreateBody(m_pPhysicsWorld, b2_dynamicBody, false, true);

    
    //Enables all GameObject components
    for (auto& i : m_Objects)
    {
        i->Enable(m_ComponentManager);
    }

    //Joint Creation
    b2RevoluteJointDef jointDef;
    //b2Vec2 anchorPos(6, 1);
    //jointDef.Initialize(m_Objects[2]->GetBody(), m_pPlayer->GetBody(), anchorPos);

    jointDef.bodyA = m_Objects[1]->GetComponent<fw::PhysicsComponent>()->GetBody();
    jointDef.bodyB = m_Objects[2]->GetComponent<fw::PhysicsComponent>()->GetBody();
    jointDef.localAnchorA = b2Vec2(0, 0);
    jointDef.localAnchorB = b2Vec2(0, -5);
    jointDef.collideConnected = false;
    jointDef.motorSpeed = 2000.0f;
    jointDef.enableMotor = true;

    m_pPhysicsWorld->CreateJoint(&jointDef);

}

FirstScene::~FirstScene()
{
    delete m_pDebugDraw;

}

void FirstScene::StartFrame(float deltaTime)
{

}

void FirstScene::OnEvent(fw::Event* pEvent)
{
    // Process Collision Events.
    if (pEvent->GetType() == fw::CollisionEvent::GetStaticEventType())
    {
        fw::CollisionEvent* pCollisonEvent = static_cast<fw::CollisionEvent*>(pEvent);

        
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

void FirstScene::Update(float deltaTime)
{

    std::vector<fw::Component*> playerComponent = m_ComponentManager->GetComponentVector<PlayerLogicComponent>();

    for (auto& i : playerComponent)
    {
        PlayerLogicComponent* player = static_cast<PlayerLogicComponent*>(i);

        player->UpdatePlayer();
    }


    //deltaTime /= 4.0f;
    for (fw::GameObject* pObject : m_Objects)
    {
        pObject->Update(deltaTime);
    }

    if (static_cast<Game*>(m_pGameCore)->GetGameState() == GameState::Play)
    {
        m_TimeSinceLastPhysicsStep += deltaTime;

        while (m_TimeSinceLastPhysicsStep >= 1 / 60.0f)
        {
            m_pPhysicsWorld->Step(1 / 60.0f, 8, 3);
            m_TimeSinceLastPhysicsStep -= 1 / 60.0f;
        }
        m_ComponentManager->SyncPhysics();
    }
    else
    {
        m_ComponentManager->SyncPhysicsToTransform();
    }


}

void FirstScene::Draw(int viewID)
{

    for (auto& i : m_ComponentManager->GetComponentVector<fw::CameraComponent>())
    {
        fw::CameraComponent* pCamera = static_cast<fw::CameraComponent*>(i);
        vec2 gameSize = static_cast<Game*>(m_pGameCore)->GetGameWindowSize();
        pCamera->SetAspectRatio(gameSize.x / gameSize.y);
    }


    m_ComponentManager->UpdateCamera();

    m_ComponentManager->UpdateTransforms();
    m_ComponentManager->RenderAll(viewID);

    m_pPhysicsWorld->DebugDraw();

}

void FirstScene::Editor_DisplayObjectList()
{

    ImGui::Begin("Object List");

    Game* pGame = static_cast<Game*>(m_pGameCore);

    for( fw::GameObject* pObject : m_Objects )
    {

        bool isSelected = (pObject == pGame->GetSelectedGameObject());
        if (ImGui::Selectable(pObject->GetName().c_str(), &isSelected))
        {
            pGame->SetIsSelected(pObject);
        }
    }

    ImGui::End(); // "Object List"
}