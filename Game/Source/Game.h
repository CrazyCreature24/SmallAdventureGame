//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "Framework.h"
#include "DataTypes.h"

class Player;
class Controller;
class MyContactListener;

enum EditorViews
{
    EditorView_Game,
    EditorView_Box2D_DebugDraw,
    EditorView_ImGui
};

enum class GameState
{
    Stop,
    Play
};


class Game : public fw::GameCore
{
public:
    Game(fw::FWCore& fwCore);
    virtual ~Game() override;

    virtual void StartFrame(float deltaTime) override;
    virtual void OnEvent(fw::Event* pEvent) override;
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;

    vec2 GetGameWindowSize() { return m_GameWindowSize; }
    fw::GameObject* GetSelectedGameObject() { return m_pSelectedGameObject; }
    GameState GetGameState() { return m_GameState; }

    void SetIsSelected(fw::GameObject* pGameObject) { m_pSelectedGameObject = pGameObject; }
protected:
    // Interface.
    fw::ImGuiManager* m_pImGuiManager = nullptr;

    //Scene
    std::map<std::string, fw::Scene*> m_pScenes;
    fw::Scene* m_pCurrentScene = nullptr;


    // GameObjects and Helpers.
    Controller* m_pController = nullptr;

    GameState m_GameState = GameState::Play;


    //Editor variables
    bgfx::TextureHandle m_FBOTexture;
    bgfx::TextureHandle m_FBODepth;
    bgfx::FrameBufferHandle m_FBO;
    vec2 m_GameWindowSize = { 0,0 };
    void Editor_DrawGameView();
    void Editor_CreateMainFrame();
    void Editor_DisplayMainMenu();
    void Editor_DisplayObjectList();
    void Editor_DrawInspector();
    fw::GameObject* m_pSelectedGameObject = nullptr;
    void SaveScene();
    void LoadScene();
};
