#pragma once


namespace fw {

    class Component;


    class ComponentManager
    {
    public:

        ComponentManager();
        virtual ~ComponentManager();

        void AddComponent(Component* pComponent);
        void RemoveComponent(Component* pComponent);

        template<typename Type>
        std::vector<Component*>& GetComponentVector()
        {
            return m_Components[Type::GetStaticType()];
        }

        //Updates all transforms for all TransformComponents
        void UpdateTransforms();

        //Renders all RenderComponents. This requires the GameOject to have a TransformComponent
        void RenderAll(int viewID);

        //Syncs all of the PhysicsComponents if they have a physics body.  This requires the GameOject to have a TransformComponent.
        void SyncPhysics(bool isXY = true);

        void SyncPhysicsToTransform(bool isXY = true);

        //Updates all CameraComponents. This requires the GameObject to have a TransformComponent.
        void UpdateCamera();

        void UpdateLights();

    protected:

        std::map<const char*, std::vector<Component*>> m_Components;
    };


}