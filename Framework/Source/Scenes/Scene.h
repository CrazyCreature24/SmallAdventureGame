#pragma once

#include "CoreHeaders.h"
#include "DataTypes.h"
#include "nlohmann/json.hpp"


class Controller;

namespace fw {

    class GameObject;
    class ComponentManager;
    class MyContactListener;
    class GameCore;
    class ResourceManager;
    class Event;
    

    class Scene
    {
    public:

        Scene(GameCore* pGameCore, Controller* pController);
        virtual ~Scene();

        virtual void StartFrame(float deltaTime) = 0;
        virtual void OnEvent(Event* pEvent) = 0;
        virtual void Update(float deltaTime) = 0;
        virtual void Draw(int viewID) = 0;

        GameCore* GetGameCore() { return m_pGameCore; }
        b2World* GetPhysicsWorld() { return m_pPhysicsWorld; }
        ComponentManager* GetComponentManager() { return m_ComponentManager; }

        virtual void Editor_DisplayObjectList();

        virtual void SaveScene(nlohmann::json& json);
        virtual void LoadScene(nlohmann::json& json);

    protected:
        GameCore* m_pGameCore = nullptr;
        Controller* m_pController = nullptr;
        ResourceManager* m_pResourceManager = nullptr;
        
        std::vector<fw::GameObject*> m_Objects;

        ComponentManager* m_ComponentManager = nullptr;
        MyContactListener* m_pContactListener = nullptr;

        b2World* m_pPhysicsWorld = nullptr;

    };
}