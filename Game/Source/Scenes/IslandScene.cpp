#include "IslandScene.h"
#include "Events/GameEvents.h"
#include "DebugDraw/MyDebugDraw.h"
#include "GameplayComponents/GameComponents.h"
#include "Game.h"


IslandScene::IslandScene(fw::GameCore* pGameCore, Controller* pController) : Scene(pGameCore, pController)
{
    m_ComponentManager = new fw::ComponentManager();

    m_pResourceManager = m_pGameCore->GetResourceManager();

    //Object Creation
    m_Objects.push_back(new fw::GameObject(this, "Player"));
    m_Objects.push_back(new fw::GameObject(this, "Height Map"));
    m_Objects.push_back(new fw::GameObject(this, "Water Map"));

    //Car Components
    fw::TransformComponent* playerTransform = new fw::TransformComponent(m_Objects[0], vec3(15, 1, 0), vec3(0, 0, 0), vec3(1.5f, 1.5f, 1.5f));
    fw::RenderComponent* playerRender = new fw::RenderComponent(m_Objects[0], m_pResourceManager->GetMesh("KnightOBJ"), m_pResourceManager->GetMaterial("Knight"));
    OrbitCameraComponent* cameraComp = new OrbitCameraComponent(m_Objects[0], vec3(10, 0, 0));
    cameraComp->SetController(m_pController);
    Player3DLogicComponent* playerLogic = new Player3DLogicComponent(m_Objects[0]);
    playerLogic->SetController(m_pController);
    fw::LightComponent* playerLight = new fw::LightComponent(m_Objects[0], vec3(0, 1, 0), 10);
    m_pActiveOrbitCamera = cameraComp;

    //Height Map
    fw::TransformComponent* mapTransform = new fw::TransformComponent(m_Objects[1], vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1));
    HeightMapComponent* mapRender = new HeightMapComponent(m_Objects[1], m_pResourceManager->GetMesh("Normals"), m_pResourceManager->GetMaterial("HeightMap123"), vec3(45, 4, 45), "Data/Textures/HeightMap1.png");
    m_pHeightMapComp = mapRender;
    playerLogic->SetHeightMap(m_pHeightMapComp);

    //Water Map
    fw::TransformComponent* waterTransform = new fw::TransformComponent(m_Objects[2], vec3(0, 1, 0), vec3(0, 0, 0), vec3(1, 1, 1));
    WaterComponent* waterRender = new WaterComponent(m_Objects[2], m_pResourceManager->GetMesh("Normals"), m_pResourceManager->GetMaterial("Water"), vec3(45, 0, 45), ivec2(130,40));

    // Sun
    fw::GameObject* object = new fw::GameObject(this, "Sun");
    fw::TransformComponent* lightTransform = new fw::TransformComponent(object, vec3(22.5f, 0, 22.5f), vec3(0, 0, 0), vec3(1, 1, 1));
    fw::LightComponent* light = new fw::LightComponent(object, vec3(1, 1, 1), 150);
    fw::RenderComponent* lightender = new fw::RenderComponent(object, m_pResourceManager->GetMesh("SunOBJ"), m_pResourceManager->GetMaterial("Yellow"));
    OrbitObjectComponent* lightOrbit = new OrbitObjectComponent(object, 30.0f, 20.0f);
    m_pLights.push_back(object);

    // Create Torches
    // Increase the amount of loops for more torches
    for (int i = 0; i < 6; i++)
    {
        CreateLight(vec3(-200, 7, -200));
    }
    
    //Enables all GameObject components
    for (auto& i : m_Objects)
    {
        i->Enable(m_ComponentManager);
    }

    for (auto& i : m_pLights)
    {
        i->Enable(m_ComponentManager);
    }

}

void IslandScene::CreateLight(vec3 position, vec3 lightColor)
{
    fw::GameObject* object1 = new fw::GameObject(this, "torch");
    fw::TransformComponent* lightTransform1 = new fw::TransformComponent(object1, position, vec3(0, 0, 0), vec3(0.5f, 0.5f, 0.5f));
    fw::LightComponent* light1 = new fw::LightComponent(object1, lightColor, 50);
    fw::RenderComponent* lightRender = new fw::RenderComponent(object1, m_pResourceManager->GetMesh("TorchOBJ"), m_pResourceManager->GetMaterial("Torch"));

    m_pLights.push_back(object1);
}

IslandScene::~IslandScene()
{
    for (fw::GameObject* pObject : m_pLights)
    {
        delete pObject;
    }
}

void IslandScene::StartFrame(float deltaTime)
{

}

void IslandScene::OnEvent(fw::Event* pEvent)
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

        m_pActiveOrbitCamera->SetAspectRatio( (float)width/height );
    }
}

void IslandScene::Update(float deltaTime)
{

    std::vector<fw::Component*> playerComponents = m_ComponentManager->GetComponentVector<Player3DLogicComponent>();

    for (auto& i : playerComponents)
    {
        Player3DLogicComponent* player = static_cast<Player3DLogicComponent*>(i);

        player->UpdatePlayer(deltaTime);
    }

    for (fw::GameObject* pObject : m_Objects)
    {
        pObject->Update(deltaTime);
    }

    
    std::vector<fw::Component*> orbitComponents = m_ComponentManager->GetComponentVector<OrbitObjectComponent>();

    for (auto& i : orbitComponents)
    {
        OrbitObjectComponent* orbitComp = static_cast<OrbitObjectComponent*>(i);

        orbitComp->UpdateOrbit(deltaTime);
    }

}

void IslandScene::Draw(int viewID)
{

    for (auto& i : m_ComponentManager->GetComponentVector<OrbitCameraComponent>())
    {
        OrbitCameraComponent* pCamera = static_cast<OrbitCameraComponent*>(i);
        vec2 gameSize = static_cast<Game*>(m_pGameCore)->GetGameWindowSize();
        pCamera->SetAspectRatio(gameSize.x / gameSize.y);

        pCamera->UpdateOrbitCamera();
    }

    m_ComponentManager->UpdateLights();

    m_ComponentManager->UpdateTransforms();
    m_ComponentManager->RenderAll(viewID);

}

void IslandScene::Editor_DisplayObjectList()
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

    for (fw::GameObject* pObject : m_pLights)
    {

        bool isSelected = (pObject == pGame->GetSelectedGameObject());
        if (ImGui::Selectable(pObject->GetName().c_str(), &isSelected))
        {
            pGame->SetIsSelected(pObject);
        }
    }

    ImGui::End(); // "Object List"
}