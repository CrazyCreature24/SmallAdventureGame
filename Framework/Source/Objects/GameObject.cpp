//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"

#include "Camera.h"
#include "GameCore.h"
#include "../Components/Component.h"
#include "../Components/ComponentManager.h"
#include "GameObject.h"
#include "Renderer/Mesh.h"
#include "Renderer/Uniforms.h"

namespace fw {


    GameObject::GameObject(Scene* pScene, std::string name)
        : m_pScene(pScene)
        , m_Name(name)

    {

    }

    GameObject::~GameObject()
    {
        for (auto& i : m_Components)
        {
            delete i;
        }
    }

    void GameObject::Update(float deltaTime)
    {


    }


    void GameObject::AddComponents(Component* pComponent)
    {
        m_Components.push_back(pComponent);
    }

    void GameObject::RemoveComponent(Component* pComponent)
    {
        for (int i = 0; i < m_Components.size(); i++)
        {
            if (m_Components[i] == pComponent)
            {
                m_Components.erase(m_Components.begin() + i);
                i--;
            }
        }

    }


    void GameObject::Enable(ComponentManager* pComponentManager)
    {
        for (auto& i : m_Components)
        {
            pComponentManager->AddComponent(i);
        }
    }


    void GameObject::Disable(ComponentManager* pComponentManager)
    {

        for (auto& i : m_Components)
        {
            pComponentManager->RemoveComponent(i);
        }
    }

    void GameObject::AddToInspector()
    {
        for (Component* pComponent : m_Components)
        {
            pComponent->AddToInspector();
        }
    }

    void GameObject::SaveToJSON(nlohmann::json& json)
    {
        json["Components"] = nlohmann::json::array();

        for (Component* pComponent : m_Components)
        {
            nlohmann::json jGameObject;
            pComponent->SaveToJSON(jGameObject);
            json["Components"].push_back(jGameObject);
        }
    }

    void GameObject::LoadFromJSON(nlohmann::json& json)
    {
        for (Component* pComponent : m_Components)
        {
            pComponent->LoadFromJSON(json);
        }
    }

} // namespace fw
