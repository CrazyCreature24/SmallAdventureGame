#pragma once

#include "Framework.h"
#include "DataTypes.h"

class MyDebugDraw;
class OrbitCameraComponent;
class HeightMapComponent;

class IslandScene : public fw::Scene
{
public:

    IslandScene(fw::GameCore* pGameCore, Controller* pController);
    virtual ~IslandScene();

    virtual void StartFrame(float deltaTime) override;
    virtual void OnEvent(fw::Event* pEvent) override;
    virtual void Update(float deltaTime) override;
    virtual void Draw(int viewID) override;

    virtual void Editor_DisplayObjectList() override;
    void CreateLight(vec3 position, vec3 lightColor = vec3(0,0,0));

protected:

    OrbitCameraComponent* m_pActiveOrbitCamera = nullptr;
    HeightMapComponent* m_pHeightMapComp = nullptr;

    std::vector<fw::GameObject*> m_pLights;

};