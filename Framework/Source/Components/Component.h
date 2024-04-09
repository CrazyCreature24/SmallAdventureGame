#pragma once
#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "../Libraries/nlohmann/json.hpp"


namespace fw {

    class GameObject;
    class Mesh;
    class Material;

    class Component
    {
    public:

        Component(GameObject* pGameObject);
        virtual ~Component();

        GameObject* m_pGameObject = nullptr;
        
        virtual const char* GetType() = 0;
        GameObject* GetOwner() { return m_pGameObject; }

        virtual void AddToInspector();
        virtual void SaveToJSON(nlohmann::json& json);
        virtual void LoadFromJSON(nlohmann::json& json);

    };


    class TransformComponent : public Component
    {
        friend class ComponentManager;
    public:

        TransformComponent(GameObject* pGameObject, vec3 position, vec3 rotation, vec3 scale);
        virtual ~TransformComponent();

        static const char* GetStaticType() { return "TransformComponent"; }
        virtual const char* GetType() override { return GetStaticType(); }

        //Getters and setters
        vec3 GetPosition() { return m_Position; }
        vec3 GetRotation() { return m_Rotation; }
        vec3 GetScale() { return m_Scale; }
        mat4 GetTransform() { return m_Transform; }
        
        void SetPosition(vec3 position) { m_Position = position; }
        void SetRotation(vec3 rotation) { m_Rotation = rotation; }
        void SetScale(vec3 scale) { m_Scale = scale; }

        virtual void AddToInspector() override;

        virtual void SaveToJSON(nlohmann::json& json) override;
        virtual void LoadFromJSON(nlohmann::json& json) override;

    protected:

        void UpdateTransform();

        vec3 m_Position = { 0,0,0 };
        vec3 m_Rotation = { 0,0,0 };
        vec3 m_Scale = { 1,1,1 };

        mat4 m_Transform;
    };


    class RenderComponent : public Component
    {
        friend class ComponentManager;
    public:

        RenderComponent(GameObject* pGameObject, Mesh* pMesh, Material* pMaterial );
        virtual ~RenderComponent();

        Mesh* GetMesh() { return m_pMesh; }
        Material* GetMaterial() { return m_pMaterial; }

        void SetMesh(Mesh* pMesh) { m_pMesh = pMesh; }
        void SetMaterial(Material* pMaterial) { m_pMaterial = pMaterial; }

        static const char* GetStaticType() { return "RenderComponent"; }
        virtual const char* GetType() override { return GetStaticType(); }

        virtual void AddToInspector() override;

        virtual void SaveToJSON(nlohmann::json& json) override;
        virtual void LoadFromJSON(nlohmann::json& json) override;

    protected:

        Mesh* m_pMesh = nullptr;
        Material* m_pMaterial = nullptr;


    };


    class PhysicsComponent : public Component
    {
        friend class ComponentManager;
    public:

        PhysicsComponent(GameObject* pGameObject);
        virtual ~PhysicsComponent();

        void CreateBody(b2World* pWorld, b2BodyType type, bool bIsSensor, bool bIs3D = false);

        b2Body* GetBody() { return m_pBody; }

        static const char* GetStaticType() { return "PhysicsComponent"; }
        virtual const char* GetType() override { return GetStaticType(); }

        virtual void AddToInspector() override;

        virtual void SaveToJSON(nlohmann::json& json) override;
        virtual void LoadFromJSON(nlohmann::json& json) override;

    protected:

        b2Body* m_pBody = nullptr;

    };


    class CameraComponent : public Component
    {
        friend class ComponentManager;
    public:

        CameraComponent(GameObject* pGameObject, vec3 relativePosition);
        virtual ~CameraComponent();

        static const char* GetStaticType() { return "CameraComponent"; }
        virtual const char* GetType() override { return GetStaticType(); }

        //Getters and setters
        vec3 GetRelativePosition() { return m_RelativePosition; }
        vec2 GetProjectionScale() { return m_ProjectionScale; }
        float32 GetAspectRatio() { return m_AspectRatio; }

        void SetRelativePosition(vec3 position) { m_RelativePosition = position; }
        void SetProjectionScale(vec2 scale) { m_ProjectionScale = scale; }
        void SetAspectRatio(float32 aspectRatio) { m_AspectRatio = aspectRatio; }
        void SetGameObject(fw::GameObject* pGameObject) { m_pGameObject = pGameObject; }

        virtual void AddToInspector() override;

        virtual void SaveToJSON(nlohmann::json& json) override;
        virtual void LoadFromJSON(nlohmann::json& json) override;

    protected:

        vec3 m_RelativePosition = { 0,0,0 };
        vec2 m_ProjectionScale = { 0.5f, 0.5f };
        float32 m_AspectRatio = 1.0f;

    };



    class LightComponent : public Component
    {
    public:

        LightComponent(fw::GameObject* pGameObject, vec3 lightColor, float lightRangeX, int arrayIndex = 0);
        ~LightComponent();

        static const char* GetStaticType() { return "LightComponent"; }
        virtual const char* GetType() override { return GetStaticType(); }

        virtual void AddToInspector() override;

        virtual void SaveToJSON(nlohmann::json& json) override;
        virtual void LoadFromJSON(nlohmann::json& json) override;

    public:

        vec4 m_LightColor = { 1,1,1,1 };
        vec4 m_LightRange = { 1,1,1,1 };
        vec4 m_LightPosition = { 0,0,0,0 };

    };


}