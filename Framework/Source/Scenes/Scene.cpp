

#include "Scenes/Scene.h"
#include "Utility/ResourceManager.h"
#include "EventSystem/Events.h"
#include "Components/ComponentManager.h"
#include "ContactListener/MyContactListener.h"
#include "GameCore.h"



namespace fw {



    Scene::Scene(GameCore* pGameCore, Controller* pController) :
        m_pGameCore(pGameCore),
        m_pController(pController)
    {

    }

    Scene::~Scene()
    {
        for (fw::GameObject* pObject : m_Objects)
        {
            delete pObject;
        }

        delete m_ComponentManager;

        delete m_pContactListener;

        delete m_pPhysicsWorld;
    }

    void Scene::Editor_DisplayObjectList()
    {
    }

    void Scene::SaveScene(nlohmann::json& json)
    {
        json["Objects"] = nlohmann::json::array();

        for (GameObject* pObject : m_Objects)
        {
            nlohmann::json jGameObject;
            pObject->SaveToJSON(jGameObject);
            json["Objects"].push_back(jGameObject);
        }
    }

    void Scene::LoadScene(nlohmann::json& json)
    {
        for (GameObject* pObject : m_Objects)
        {
            pObject->LoadFromJSON(json);
        }
    }


}