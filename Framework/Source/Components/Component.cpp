#include "CoreHeaders.h"
#include "../Objects/GameObject.h"
#include "GameCore.h"
#include "Renderer/Mesh.h"
#include "Renderer/Material.h"
#include "Renderer/Uniforms.h"
#include "Scenes/Scene.h"
#include "FWCore.h"
#include "Imgui/ImGui.h"
#include "Utility/ResourceManager.h"
#include "Component.h"

namespace fw {

    Component::Component(GameObject* pGameObject) : m_pGameObject(pGameObject)
    {
        m_pGameObject->AddComponents(this);
    }

    Component::~Component()
    {

    }

    void Component::AddToInspector()
    {

    }

    void Component::SaveToJSON(nlohmann::json& json)
    {
        json["Type"] = GetType();
    }

    void Component::LoadFromJSON(nlohmann::json& json)
    {

    }

    ///////////////////////////////////////////////////////////////

    TransformComponent::TransformComponent(GameObject* pGameObject, vec3 position, vec3 rotation, vec3 scale) :
        Component(pGameObject),
        m_Position(position),
        m_Rotation(rotation),
        m_Scale(scale)
    {

    }

    TransformComponent::~TransformComponent()
    {

    }


    void TransformComponent::AddToInspector()
    {
        ImGui::DragFloat3("Position", &m_Position.x);
        ImGui::DragFloat3("Rotation", &m_Rotation.x);
        ImGui::DragFloat3("Scale", &m_Scale.x);
    }

    void TransformComponent::SaveToJSON(nlohmann::json& json)
    {
        Component::SaveToJSON(json);


        json["Position"] = { m_Position.x, m_Position.y, m_Position.z };
        json["Rotation"] = { m_Rotation.x, m_Rotation.y, m_Rotation.z };
        json["Scale"] = { m_Scale.x, m_Scale.y, m_Scale.z };



    }

    void TransformComponent::LoadFromJSON(nlohmann::json& json)
    {
        m_Position = vec3(json["Position"][0], json["Position"][1], json["Position"][2]);
        m_Rotation = vec3(json["Rotation"][0], json["Rotation"][1], json["Rotation"][2]);
        m_Scale = vec3(json["Scale"][0], json["Scale"][1], json["Scale"][2]);
    }

    void TransformComponent::UpdateTransform()
    {
        m_Transform.CreateSRT(m_Scale, m_Rotation, m_Position);
    }

    /////////////////////////////////////////////////////////

    RenderComponent::RenderComponent(GameObject* pGameObject, Mesh* pMesh, Material* pMaterial) :
        Component(pGameObject),
        m_pMesh(pMesh),
        m_pMaterial(pMaterial)
    {

    }

    RenderComponent::~RenderComponent()
    {

    }

    void RenderComponent::AddToInspector()
    {

    }

    void RenderComponent::SaveToJSON(nlohmann::json& json)
    {
        Component::SaveToJSON(json);

        //TODO: Optimize later. Hint you have a gamecore pointer.

        json["Mesh"] = { m_pGameObject->GetScene()->GetGameCore()->GetResourceManager()->GetMeshName(m_pMesh) };
        json["Material"] = { m_pGameObject->GetScene()->GetGameCore()->GetResourceManager()->GetMaterialName(m_pMaterial) };


    }

    void RenderComponent::LoadFromJSON(nlohmann::json& json)
    {
        m_pMesh = m_pGameObject->GetScene()->GetGameCore()->GetResourceManager()->GetMesh(json["Mesh"]);
        m_pMaterial = m_pGameObject->GetScene()->GetGameCore()->GetResourceManager()->GetMaterial(json["Material"]);
    }

    //////////////////////////////////////////////////////

    PhysicsComponent::PhysicsComponent(GameObject* pGameObject) : Component(pGameObject)
    {
    }

    PhysicsComponent::~PhysicsComponent()
    {
        if (m_pBody)
        {
            GetOwner()->GetScene()->GetPhysicsWorld()->DestroyBody(m_pBody);
        }
    }

    void PhysicsComponent::CreateBody(b2World* pWorld, b2BodyType type, bool bIsSensor, bool bIs3D)
    {
        TransformComponent* pTransform = m_pGameObject->GetComponent<TransformComponent>();
        assert(pTransform != nullptr);

        b2BodyDef bodyDef;
        bodyDef.type = type;
        bodyDef.position.Set(pTransform->GetPosition().x, pTransform->GetPosition().y);
        bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(m_pGameObject);
        m_pBody = pWorld->CreateBody(&bodyDef);

        b2PolygonShape boxShape;
        if (bIs3D)
        {
            boxShape.SetAsBox(pTransform->GetScale().x, pTransform->GetScale().y);
        }
        else
        {
            boxShape.SetAsBox(pTransform->GetScale().x / 2, pTransform->GetScale().y / 2);
        }

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &boxShape;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        fixtureDef.isSensor = bIsSensor;

        m_pBody->CreateFixture(&fixtureDef);
    }

    void PhysicsComponent::AddToInspector()
    {
        float angle = fw::radsToDegrees(m_pBody->GetAngle());
        ImGui::DragFloat("Physics Angle", &angle);
    }

    void PhysicsComponent::SaveToJSON(nlohmann::json& json)
    {
        Component::SaveToJSON(json);

        json["BodyType"] = m_pBody->GetType();
    }

    void PhysicsComponent::LoadFromJSON(nlohmann::json& json)
    {
        m_pBody->SetType(json["BodyType"]);
    }

    ///////////////////////////////////////////////////////////


    CameraComponent::CameraComponent(GameObject* pGameObject, vec3 relativePosition) :
        Component(pGameObject),
        m_RelativePosition(relativePosition)
    {
        FWCore* pFW = m_pGameObject->GetScene()->GetGameCore()->GetFramework();
        SetAspectRatio((float)pFW->GetWindowClientWidth() / pFW->GetWindowClientHeight());
    }

    CameraComponent::~CameraComponent()
    {
    }

    void CameraComponent::AddToInspector()
    {
        ImGui::DragFloat3("RelPosition", &m_RelativePosition.x);
    }

    void CameraComponent::SaveToJSON(nlohmann::json& json)
    {
        Component::SaveToJSON(json);

        json["RelPosition"] = { m_RelativePosition.x, m_RelativePosition.y, m_RelativePosition.z };
        json["AspectRatio"] = { m_AspectRatio };
    }

    void CameraComponent::LoadFromJSON(nlohmann::json& json)
    {
        m_RelativePosition = vec3(json["RelPosition"][0], json["RelPosition"][1], json["RelPosition"][2]);
        m_AspectRatio = json["AspectRatio"];
    }


    //////////////////////////////////////////////////////

    LightComponent::LightComponent(fw::GameObject* pGameObject, vec3 lightColor, float lightRangeX, int arrayIndex) :
        Component(pGameObject),
        m_LightColor(lightColor, 0),
        m_LightRange(lightRangeX, 0, 0, 0)
    {
        fw::TransformComponent* pTransform = m_pGameObject->GetComponent<fw::TransformComponent>();
        assert(pTransform != nullptr);

        m_LightPosition = vec4(pTransform->GetPosition(), 0);

        
    }

    LightComponent::~LightComponent()
    {
        
    }

    void LightComponent::AddToInspector()
    {
        ImGui::DragFloat("LightColorR", &m_LightColor.x, 0.02f, 0.0f, 1.0f);
        ImGui::DragFloat("LightColorG", &m_LightColor.y, 0.02f, 0.0f, 1.0f);
        ImGui::DragFloat("LightColorB", &m_LightColor.z, 0.02f, 0.0f, 1.0f);
        ImGui::DragFloat("LightRange", &m_LightRange.x, 1.0f, 0.0f, 1000.0f);
    }

    void LightComponent::SaveToJSON(nlohmann::json& json)
    {

    }

    void LightComponent::LoadFromJSON(nlohmann::json& json)
    {

    }

}
