#pragma once

#include "Framework.h"
#include "DataTypes.h"
#include "GameplayComponents/GameComponents.h"

class MyDebugDraw;

class Mariokart : public fw::Scene
{
public:

    Mariokart(fw::GameCore* pGameCore, Controller* pController);
    virtual ~Mariokart();

    virtual void StartFrame(float deltaTime) override;
    virtual void OnEvent(fw::Event* pEvent) override;
    virtual void Update(float deltaTime) override;
    virtual void Draw(int viewID) override;

    void CreateKart(vec3 position, const char* MaterialName);
    void BuildTrackCollision();
    void ChangeActiveKart(fw::GameObject* kart);

    void CreateSensorObject(SensorComponent::SensorType type, vec3 position);
    void HandleSensorCollision(fw::GameObject* pObjectSensor, fw::GameObject* pObjectKart);

    fw::GameObject* GetCoinObject() { return m_pCoinObject; }

    void Editor_DisplayObjectList();

protected:

    MyDebugDraw* m_pDebugDraw = nullptr;

    float m_TimeSinceLastPhysicsStep = 0;

    std::vector<fw::GameObject*> m_pTrackCollision;
    std::vector<fw::GameObject*> m_pKarts;
    std::vector<fw::GameObject*> m_pSensorObjects;

    fw::CameraComponent* m_pActiveCamera = nullptr;
    fw::GameObject* m_pActiveKart = nullptr;
    
    // For floating coin object that floats above player on pickup.
    fw::GameObject* m_pCoinObject;
    fw::GameObject* m_pCoinParent;

};