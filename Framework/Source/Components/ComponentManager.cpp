#include "CoreHeaders.h"
#include "Component.h"
#include "Renderer/Mesh.h"
#include "../Objects/GameObject.h"
#include "Renderer/Uniforms.h"
#include "GameCore.h"
#include "Math/MathHelpers.h"
#include "Scenes/Scene.h"
#include "ComponentManager.h"


namespace fw {

    ComponentManager::ComponentManager()
    {


    }

    ComponentManager::~ComponentManager()
    {

    }

    void ComponentManager::AddComponent(Component* pComponent)
    {
        std::vector<Component*>& list = m_Components[pComponent->GetType()];

        //assert(std::find(list.begin(), list.end(), pComponent) == list.end());
        if (std::find(list.begin(), list.end(), pComponent) == list.end())
            list.push_back(pComponent);
    }

    void ComponentManager::RemoveComponent(Component* pComponent)
    {
        std::vector<Component*>& list = m_Components[pComponent->GetType()];

        list.erase(std::remove(list.begin(), list.end(), pComponent), list.end());

    }

    void ComponentManager::UpdateTransforms()
    {
        for (Component* i : m_Components[TransformComponent::GetStaticType()])
        {
            TransformComponent* pTransform = static_cast<TransformComponent*>(i);

            pTransform->UpdateTransform();

        }

    }

    void ComponentManager::RenderAll(int viewID)
    {
        for (Component* i : m_Components[RenderComponent::GetStaticType()])
        {
            RenderComponent* pRender = static_cast<RenderComponent*>(i);

            Mesh* pMesh = pRender->GetMesh();
            Material* pMaterial = pRender->GetMaterial();
            GameObject* pGameObject = pRender->GetOwner();
            GameCore* pGameCore = pGameObject->GetScene()->GetGameCore();
            Uniforms* pUniforms = pGameCore->GetUniforms();

            TransformComponent* pTransform = pGameObject->GetComponent<TransformComponent>();
            assert(pTransform != nullptr);

            mat4 worldMatrix = pTransform->GetTransform();

            bgfx::setUniform(pUniforms->GetUniform("u_WorldMatrix"), &worldMatrix);

            mat4 normalMatrix;
            normalMatrix.CreateRotation(pGameObject->GetComponent<TransformComponent>()->GetRotation());
            bgfx::setUniform(pUniforms->GetUniform("u_NormalMatrix"), &normalMatrix);

            pMesh->Draw(viewID, pUniforms, pMaterial);

        }
    }

    void ComponentManager::SyncPhysics(bool isXY)
    {
        for (Component* i : m_Components[PhysicsComponent::GetStaticType()])
        {
            PhysicsComponent* pPhysics = static_cast<PhysicsComponent*>(i);

            if (pPhysics->m_pBody)
            {
                GameObject* pGameObject = pPhysics->GetOwner();
                TransformComponent* pTransform = pGameObject->GetComponent<TransformComponent>();
                assert(pTransform != nullptr);

                b2Vec2 pos = pPhysics->m_pBody->GetPosition();
                float rot = pPhysics->m_pBody->GetAngle() * 180 / PI;

                if (isXY)
                {
                    //pTransform->SetPosition(vec3(pos.x, pos.y, 0));
                    pTransform->m_Position.x = pos.x;
                    pTransform->m_Position.y = pos.y;
                    pTransform->SetRotation(vec3(0, 0, -rot));
                }
                else
                {
                    //pTransform->SetPosition(vec3(pos.x, 0, pos.y));
                    pTransform->m_Position.x = pos.x;
                    pTransform->m_Position.z = pos.y;

                    pTransform->SetRotation(vec3(0, rot, 0));
                }

            }

        }

    }

    void ComponentManager::SyncPhysicsToTransform(bool isXY)
    {
        for (Component* i : m_Components[PhysicsComponent::GetStaticType()])
        {
            PhysicsComponent* pPhysics = static_cast<PhysicsComponent*>(i);

            if (pPhysics->m_pBody)
            {
                GameObject* pGameObject = pPhysics->GetOwner();
                TransformComponent* pTransform = pGameObject->GetComponent<TransformComponent>();
                assert(pTransform != nullptr);

                b2Vec2 pos;
                float rot;

                if (isXY)
                {
                    pos = b2Vec2(pTransform->GetPosition().x, pTransform->GetPosition().y);
                    rot = degreesToRads(pTransform->GetRotation().z);
                }
                else
                {
                    pos = b2Vec2(pTransform->GetPosition().x, pTransform->GetPosition().z);
                    rot = degreesToRads(pTransform->GetRotation().y);
                }


                pPhysics->GetBody()->SetTransform(pos, -rot);

            }

        }
    }


    void ComponentManager::UpdateCamera()
    {
        for (Component* i : m_Components[CameraComponent::GetStaticType()])
        {
            CameraComponent* pCamera = static_cast<CameraComponent*>(i);

            GameObject* pGameObject = pCamera->GetOwner();

            Uniforms* pUniforms = pGameObject->GetScene()->GetGameCore()->GetUniforms();
            //The next line is not being used. Fix if needed.
            //vec2 scaleWithAspectRatio = pCamera->GetProjectionScale() * vec2(1.0f / pCamera->GetAspectRatio(), 1);

            TransformComponent* pTransform = pGameObject->GetComponent<TransformComponent>();
            assert(pTransform != nullptr);

            vec3 ObjectPosition = pTransform->GetPosition();
            vec3 ObjectRotation = pTransform->GetRotation();

            float distanceXZ = sqrt(pCamera->GetRelativePosition().z * pCamera->GetRelativePosition().z + pCamera->GetRelativePosition().x * pCamera->GetRelativePosition().x);
            float angleY = fw::degreesToRads(ObjectRotation.y - 90);

            float locX = cos(angleY) * distanceXZ;
            float locZ = sin(angleY) * distanceXZ;

            vec3 CameraOffset = vec3(locX, pCamera->GetRelativePosition().y, locZ);
            vec3 CameraPosition = ObjectPosition + CameraOffset;

            mat4 viewMatrix;
            vec3 eye(CameraPosition.x, CameraPosition.y, CameraPosition.z);
            vec3 up(0, 1, 0);
            vec3 at(ObjectPosition.x, ObjectPosition.y, ObjectPosition.z);
            viewMatrix.CreateLookAtView(eye, up, at);

            mat4 projMatrix;
            projMatrix.CreatePerspectiveVFoV(45, pCamera->GetAspectRatio(), 0.01f, 200.0f);

            vec4 camPos = vec4(CameraPosition, 1);

            // Setup uniforms.
            bgfx::setUniform(pUniforms->GetUniform("u_CameraPosition"), &camPos);
            bgfx::setUniform(pUniforms->GetUniform("u_ViewMatrix"), &viewMatrix);
            bgfx::setUniform(pUniforms->GetUniform("u_ProjMatrix"), &projMatrix);

        }
    }



    void ComponentManager::UpdateLights()
    {
        std::vector<Component*> lightComps = GetComponentVector<LightComponent>();

        std::vector<vec4> lightPositions;
        std::vector<vec4> lightRanges;
        std::vector<vec4> lightColors;
        
        if (lightComps.size())
        {
            // This is for if there are more lights then the uniform can handle
            if (lightComps.size() > 8)
            {
                std::vector<LightComponent*> torches;

                GameObject* player = nullptr;

                // Find the player light and all the torches, leave out the sun for now.
                for (int i = 0; i < lightComps.size(); i++)
                {
                    LightComponent* light = static_cast<LightComponent*>(lightComps[i]);
                    GameObject* owner = light->GetOwner();
                    
                    if (owner->GetName() == "Player")
                    {
                        player = owner;
                    }

                    if (owner->GetName() == "torch")
                    {
                        torches.push_back(light);
                    }
                }
                
                // Setup player Position for comparison for later
                assert(player);
                TransformComponent* pTransformPlayer = player->GetComponent<TransformComponent>();
                assert(pTransformPlayer);
                vec3 playerPos = pTransformPlayer->GetPosition();

                for (int i = 0; i < torches.size(); i++)
                {
                    // Find current light position
                    GameObject* owner = torches[i]->GetOwner();
                    TransformComponent* pTransform = owner->GetComponent<TransformComponent>();
                    assert(pTransform);

                    vec3 pos = pTransform->GetPosition();
                    float distanceFromPlayer = (pos - playerPos).Length();

                    if (i != 0)
                    {
                        // Find previous light position
                        GameObject* previousOwner = torches[i - 1]->GetOwner();
                        TransformComponent* pTransformPrevious = previousOwner->GetComponent<TransformComponent>();
                        assert(pTransformPrevious);

                        vec3 posPrev = pTransformPrevious->GetPosition();
                        //torches[i]->m_LightPosition = vec4(pos, 0);
                        //torches[i - 1]->m_LightPosition = vec4(posPrev, 0);

                        float distanceFromPlayerPrev = (posPrev - playerPos).Length();

                        // Sort so the shorter distance it at the top
                        if (distanceFromPlayerPrev < distanceFromPlayer)
                        {
                            LightComponent* temp = torches[i - 1];
                            torches[i - 1] = torches[i];
                            torches[i] = temp;
                        }
                    }

                    // Pop back unitl there is only 6 remaining
                    while (torches.size() > 6)
                    {
                        torches.pop_back();
                    }

                    // Add the sun and the player lights
                    torches.push_back(static_cast<LightComponent*>(lightComps[0]));
                    torches.push_back(static_cast<LightComponent*>(lightComps[1]));
                }

                for (int i = 0; i < torches.size(); i++)
                {

                    LightComponent* light = static_cast<LightComponent*>(torches[i]);
                    GameObject* owner = light->GetOwner();
                    TransformComponent* pTransform = owner->GetComponent<TransformComponent>();
                    assert(pTransform);
                    light->m_LightPosition = vec4(pTransform->GetPosition() + vec3(0, 0.5f, 0), 0);

                    lightPositions.push_back(light->m_LightPosition);
                    lightRanges.push_back(light->m_LightRange);
                    lightColors.push_back(light->m_LightColor);

                }
            }
            else
            {

                for (int i = 0; i < lightComps.size(); i++)
                {

                    LightComponent* light = static_cast<LightComponent*>(lightComps[i]);
                    GameObject* owner = light->GetOwner();
                    TransformComponent* pTransform = owner->GetComponent<TransformComponent>();
                    assert(pTransform);
                    light->m_LightPosition = vec4(pTransform->GetPosition() + vec3(0, 0.5f, 0), 0);

                    lightPositions.push_back(light->m_LightPosition);
                    lightRanges.push_back(light->m_LightRange);
                    lightColors.push_back(light->m_LightColor);

                }
            }
            fw::Uniforms* pUniforms = lightComps[0]->GetOwner()->GetScene()->GetGameCore()->GetUniforms();
            bgfx::setUniform(pUniforms->GetUniform("u_LightColor"), lightColors.data(), 8);
            bgfx::setUniform(pUniforms->GetUniform("u_LightRange"), lightRanges.data(), 8);
            bgfx::setUniform(pUniforms->GetUniform("u_LightPosition"), lightPositions.data(), 8);
        }

    }

}