#pragma once

#include "Framework.h"
#include "DataTypes.h"

class Player;
class MyDebugDraw;

class FirstScene : public fw::Scene
{
public:

    FirstScene(fw::GameCore* pGameCore, Controller* pController);
    virtual ~FirstScene();

    virtual void StartFrame(float deltaTime) override;
    virtual void OnEvent(fw::Event* pEvent) override;
    virtual void Update(float deltaTime) override;
    virtual void Draw(int viewID) override;

    virtual void Editor_DisplayObjectList() override;

protected:

    MyDebugDraw* m_pDebugDraw = nullptr;

    fw::CameraComponent* m_pActiveCamera = nullptr;

    float m_TimeSinceLastPhysicsStep = 0;

};