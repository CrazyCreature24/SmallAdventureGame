//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "Math/Vector.h"
#include "nlohmann/json.hpp"

namespace fw {

class Camera;
class GameCore;
class Material;
class Mesh;
class Uniforms;
class Component;
class ComponentManager;
class Scene;

class GameObject
{
public:
    GameObject(Scene* pScene, std::string name);
    virtual ~GameObject();

    virtual void Update(float deltaTime);

    // Getters.
    std::string GetName() { return m_Name; }
    Scene* GetScene() { return m_pScene; }
    std::vector<Component*> GetAllComponents() { return m_Components; }

    //Setters
    void SetName(std::string name) { m_Name = name; }


    template<typename Type> 
    Type* GetComponent()
    {
        for (Component* pComponent : m_Components)
        {
            if (pComponent->GetType() == Type::GetStaticType())
            {
                return static_cast<Type*>(pComponent);
            }
        }
        return nullptr;
    }

    void AddComponents(Component* pComponent);
    void RemoveComponent(Component* pComponent);

    //Adds all components from a specific GameObject to the ComponentManager
    void Enable(ComponentManager* pComponentManager);
    
    //Removes all components from a specific GameObject to the ComponentManager
    void Disable(ComponentManager* pComponentManager);


    virtual void AddToInspector();
    virtual void SaveToJSON(nlohmann::json& json);
    virtual void LoadFromJSON(nlohmann::json& json);

protected:

    Scene* m_pScene = nullptr;

    std::string m_Name;
    
    std::vector<Component*> m_Components;

};

} // namespace fw
