//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Framework.h"

#include "DataTypes.h"
#include "Game.h"
#include "Events/GameEvents.h"
#include "Objects/Player.h"
#include "Objects/Controller.h"
#include "Meshes/Shapes.h"
#include "Meshes/VertexFormats.h"
#include "Meshes/GameMesh.h"
#include "Scenes/FirstScene.h"
#include "Scenes/Mariokart.h"
#include "Scenes/HeightMapScene.h"
#include "Scenes/IslandScene.h"




Game::Game(fw::FWCore& fwCore)
    : GameCore( fwCore )
{
    //Create a texture and a framebuffer object.
    m_FBOTexture = bgfx::createTexture2D(4096, 4096, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_RT);
    m_FBODepth = bgfx::createTexture2D(4096, 4096, false, 1, bgfx::TextureFormat::D32, BGFX_TEXTURE_RT);
    bgfx::TextureHandle textures[] = { m_FBOTexture, m_FBODepth };
    m_FBO = bgfx::createFrameBuffer(2, textures);


    // General renderer settings.
    int viewID = 0;
    bgfx::setViewFrameBuffer(EditorView_Game, m_FBO);
    bgfx::setViewClear(EditorView_Game, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH, 0x000030ff, 1.0f, 0 );
    bgfx::setViewRect(EditorView_Game, 0, 0, m_FWCore.GetWindowClientWidth(), m_FWCore.GetWindowClientHeight() );
    
    bgfx::setViewFrameBuffer(EditorView_Box2D_DebugDraw, m_FBO);
    bgfx::setViewClear(EditorView_Box2D_DebugDraw, BGFX_CLEAR_NONE, 0x000000ff, 1.0f, 0 );
    bgfx::setViewRect(EditorView_Box2D_DebugDraw, 0, 0, m_FWCore.GetWindowClientWidth(), m_FWCore.GetWindowClientHeight() );
    
    bgfx::setViewClear(EditorView_ImGui, BGFX_CLEAR_NONE, 0x000000ff, 1.0f, 0);
    //bgfx::setViewClear(EditorView_ImGui, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH, 0x000000ff, 1.0f, 0);
    bgfx::setViewRect(EditorView_ImGui, 0, 0, m_FWCore.GetWindowClientWidth(), m_FWCore.GetWindowClientHeight());


    // Create some manager objects.
    m_pImGuiManager = new fw::ImGuiManager( &m_FWCore, EditorView_ImGui );
    m_pEventManager = new fw::EventManager();

    // Create uniforms.
    m_pUniforms = new fw::Uniforms();

    m_pUniforms->CreateUniform( "u_CameraPosition", bgfx::UniformType::Vec4 );
    
    m_pUniforms->CreateUniform( "u_DiffuseColor", bgfx::UniformType::Vec4 );
    m_pUniforms->CreateUniform( "u_TextureColor", bgfx::UniformType::Sampler );
    m_pUniforms->CreateUniform( "u_UVScale", bgfx::UniformType::Vec4 );
    m_pUniforms->CreateUniform( "u_UVOffset", bgfx::UniformType::Vec4 );

    m_pUniforms->CreateUniform( "u_Time", bgfx::UniformType::Vec4 );

    m_pUniforms->CreateUniform( "u_WorldMatrix", bgfx::UniformType::Mat4 );
    m_pUniforms->CreateUniform( "u_ViewMatrix", bgfx::UniformType::Mat4 );
    m_pUniforms->CreateUniform( "u_ProjMatrix", bgfx::UniformType::Mat4 );
    
    m_pUniforms->CreateUniform( "u_NormalMatrix", bgfx::UniformType::Mat4 );
    m_pUniforms->CreateUniform( "u_LightPosition", bgfx::UniformType::Vec4, 8);
    m_pUniforms->CreateUniform( "u_LightColor", bgfx::UniformType::Vec4, 8);
    m_pUniforms->CreateUniform( "u_LightRange", bgfx::UniformType::Vec4, 8);

    // Create vertex formats.
    InitVertexFormats();

    //Fill Resource Manager
    m_pResources = new fw::ResourceManager();

    // Create some meshes.
    m_pResources->AddMesh("Triangle", CreateTriangleMesh());
    m_pResources->AddMesh("Square", CreateSquareMesh());
    m_pResources->AddMesh("Sprite", CreateSpriteMesh());
    m_pResources->AddMesh("Cube", Create3dSpriteMesh(vec3(2, 2, 2)));
    m_pResources->AddMesh("Cube2", Create3dSpriteMesh(vec3(2, 2, 2)));
    m_pResources->AddMesh("Kart", CreateDoubleSidedSpriteMesh());
    m_pResources->AddMesh("Normals", CreateSpriteNormalMesh());
    m_pResources->AddMesh("OBJTest", new GameMesh(VertexFormat_PosUV::format, "Data/Meshes/Car.obj"));
    m_pResources->AddMesh("SunOBJ", new GameMesh(VertexFormat_PosUV::format, "Data/Meshes/Sun.obj"));
    m_pResources->AddMesh("TorchOBJ", new GameMesh(VertexFormat_PosUV::format, "Data/Meshes/Torch.obj"));
    m_pResources->AddMesh("KnightOBJ", new GameMesh(VertexFormat_PosUV::format, "Data/Meshes/Knight.obj"));

    // Load some shaders.
    m_pResources->AddShader("SolidColor", new fw::ShaderProgram("Data/Shaders/", "SolidColor.vert.bin", "SolidColor.frag.bin"));
    m_pResources->AddShader("VertexColor", new fw::ShaderProgram("Data/Shaders/", "VertexColor.vert.bin", "VertexColor.frag.bin"));
    m_pResources->AddShader("Texture", new fw::ShaderProgram("Data/Shaders/", "Texture.vert.bin", "Texture.frag.bin"));
    m_pResources->AddShader("DebugNormals", new fw::ShaderProgram("Data/Shaders/", "DebugNormals.vert.bin", "DebugNormals.frag.bin"));
    m_pResources->AddShader("Lighting", new fw::ShaderProgram("Data/Shaders/", "Lighting.vert.bin", "Lighting.frag.bin"));
    m_pResources->AddShader("Water", new fw::ShaderProgram("Data/Shaders/", "Water.vert.bin", "Water.frag.bin"));

    // Load some textures.
    m_pResources->AddTexture("MegaMan", new fw::Texture("Data/Textures/MegaMan.png"));
    m_pResources->AddTexture("Die", new fw::Texture("Data/Textures/Die.png"));
    m_pResources->AddTexture("Bowser", new fw::Texture("Data/Textures/KartBowserBoth.png"));
    m_pResources->AddTexture("Mario", new fw::Texture("Data/Textures/KartMarioBoth.png"));
    m_pResources->AddTexture("Luigi", new fw::Texture("Data/Textures/KartLuigiBoth.png"));
    m_pResources->AddTexture("Toad", new fw::Texture("Data/Textures/KartToadBoth.png"));
    m_pResources->AddTexture("Track", new fw::Texture("Data/Textures/SNES_Mario_Circuit_3_map.png"));
    m_pResources->AddTexture("Coin", new fw::Texture("Data/Textures/Coin2.png"));
    m_pResources->AddTexture("Car", new fw::Texture("Data/Textures/Car.png"));
    m_pResources->AddTexture("Sun", new fw::Texture("Data/Textures/Sun.png"));
    m_pResources->AddTexture("Torch", new fw::Texture("Data/Textures/Torch.png"));
    m_pResources->AddTexture("Knight", new fw::Texture("Data/Textures/Knight.png"));
    m_pResources->AddTexture("HeightMap", new fw::Texture("Data/Textures/HM16.png"));
    m_pResources->AddTexture("HeightMap123", new fw::Texture("Data/Textures/HeightMap1.png"));

    // Create some materials.
    m_pResources->AddMaterial("Red", new fw::Material(m_pResources->GetShader("SolidColor"), nullptr, fw::color4f::Red(), false));
    m_pResources->AddMaterial("Blue", new fw::Material(m_pResources->GetShader("SolidColor"), nullptr, fw::color4f::Blue(), false));
    m_pResources->AddMaterial("Green", new fw::Material(m_pResources->GetShader("SolidColor"), nullptr, fw::color4f::Green(), false));
    m_pResources->AddMaterial("Yellow", new fw::Material(m_pResources->GetShader("SolidColor"), nullptr, fw::color4f::Yellow(), false));
    m_pResources->AddMaterial("VertexColor", new fw::Material(m_pResources->GetShader("VertexColor"), nullptr, fw::color4f::White(), false));
    m_pResources->AddMaterial("MegaMan", new fw::Material(m_pResources->GetShader("Lighting"), m_pResources->GetTexture("MegaMan"), fw::color4f::White(), true));
    m_pResources->AddMaterial("Die", new fw::Material(m_pResources->GetShader("Texture"), m_pResources->GetTexture("Die"), fw::color4f::White(), true));
    m_pResources->AddMaterial("Bowser", new fw::Material(m_pResources->GetShader("Texture"), m_pResources->GetTexture("Bowser"), fw::color4f::White(), true));
    m_pResources->AddMaterial("Mario", new fw::Material(m_pResources->GetShader("Texture"), m_pResources->GetTexture("Mario"), fw::color4f::White(), true));
    m_pResources->AddMaterial("Luigi", new fw::Material(m_pResources->GetShader("Texture"), m_pResources->GetTexture("Luigi"), fw::color4f::White(), true));
    m_pResources->AddMaterial("Toad", new fw::Material(m_pResources->GetShader("Texture"), m_pResources->GetTexture("Toad"), fw::color4f::White(), true));
    m_pResources->AddMaterial("Track", new fw::Material(m_pResources->GetShader("Texture"), m_pResources->GetTexture("Track"), fw::color4f::White(), true));
    m_pResources->AddMaterial("Coin", new fw::Material(m_pResources->GetShader("Texture"), m_pResources->GetTexture("Coin"), fw::color4f::White(), true));
    m_pResources->AddMaterial("Car", new fw::Material(m_pResources->GetShader("Texture"), m_pResources->GetTexture("Car"), fw::color4f::White(), true));
    m_pResources->AddMaterial("Sun", new fw::Material(m_pResources->GetShader("Texture"), m_pResources->GetTexture("Sun"), fw::color4f::White(), true));
    m_pResources->AddMaterial("Torch", new fw::Material(m_pResources->GetShader("Texture"), m_pResources->GetTexture("Torch"), fw::color4f::White(), true));
    m_pResources->AddMaterial("Knight", new fw::Material(m_pResources->GetShader("Texture"), m_pResources->GetTexture("Knight"), fw::color4f::White(), true));
    m_pResources->AddMaterial("HeightMap", new fw::Material(m_pResources->GetShader("Lighting"), m_pResources->GetTexture("HeightMap"), fw::color4f::White(), true));
    m_pResources->AddMaterial("HeightMap123", new fw::Material(m_pResources->GetShader("Lighting"), m_pResources->GetTexture("HeightMap123"), fw::color4f::White(), true));
    m_pResources->AddMaterial("Water", new fw::Material(m_pResources->GetShader("Water"), m_pResources->GetTexture("HeightMap"), fw::color4f::White(), true));

    //m_pResources->GetMaterial("Water")->Debug_SetPointModeOn();

    // Create a controller.
    m_pController = new Controller();

    // Create Scenes
    m_pScenes["FirstScene"] = new FirstScene(this, m_pController);
    m_pScenes["SecondScene"] = new FirstScene(this, m_pController);
    m_pScenes["Mariokart"] = new Mariokart(this, m_pController);
    m_pScenes["HeightMapScene"] = new HeightMapScene(this, m_pController);
    m_pScenes["IslandScene"] = new IslandScene(this, m_pController);
    m_pCurrentScene = m_pScenes["IslandScene"];

}

Game::~Game()
{
    bgfx::destroy(m_FBOTexture);
    bgfx::destroy(m_FBODepth);
    bgfx::destroy(m_FBO);


    for( auto& scenePair : m_pScenes)
    {
        delete scenePair.second;
    }

    delete m_pResources;

    delete m_pController;
    delete m_pUniforms;

    delete m_pEventManager;
    delete m_pImGuiManager;
}

void Game::StartFrame(float deltaTime)
{
    m_pImGuiManager->StartFrame( deltaTime );

    // Reset the controller.
    m_pController->StartFrame();

    m_pCurrentScene->StartFrame(deltaTime);

    // Dispatch events.
    m_pEventManager->DispatchAllEvents( this );
}

void Game::OnEvent(fw::Event* pEvent)
{
    // Process events.
    m_pController->OnEvent( pEvent );

    m_pCurrentScene->OnEvent(pEvent);

    // Pass "WM_CHAR" events to imgui to handle text input.
    if( pEvent->GetType() == fw::OnCharEvent::GetStaticEventType() )
    {
        int character = static_cast<fw::OnCharEvent*>(pEvent)->GetValue();
        m_pImGuiManager->AddInputCharacter( character );
    }
}

void Game::Update(float deltaTime)
{
    m_pCurrentScene->Update(deltaTime);

    Editor_CreateMainFrame();
    Editor_DisplayMainMenu();
    Editor_DrawGameView();
    Editor_DisplayObjectList();
    Editor_DrawInspector();
    
    //ImGui::ShowDemoWindow();

}

void Game::Draw()
{
    int viewID = 0;

    // Setup time uniforms.
    float time = (float)fw::GetSystemTimeSinceGameStart();
    bgfx::setUniform( m_pUniforms->GetUniform("u_Time"), &time );

    m_pCurrentScene->Draw(EditorView_Game);

    m_pImGuiManager->EndFrame();
}


void Game::Editor_DrawGameView()
{

    int viewID = 0;

    if (ImGui::Begin("Game View"))
    {
        ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
        ImVec2 contentMax = ImGui::GetWindowContentRegionMax();
        ImVec2 size = contentMax - contentMin;
        if (size.x <= 0)
            size.x = 1;
        if (size.y <= 0)
            size.y = 1;
        
        m_GameWindowSize.Set(size.x, size.y);
        bgfx::setViewRect(EditorView_Game, 0, 0, (uint16_t)size.x, (uint16_t)size.y);
        bgfx::setViewRect(EditorView_Box2D_DebugDraw, 0, 0, (uint16_t)size.x, (uint16_t)size.y);

        vec2 uv0 = vec2(0, 1);
        vec2 uv1 = vec2(size.x /4096.0f, 1 - size.y / 4096.0f);
        ImGui::Image(fw::imguiTexture(m_FBOTexture), size, uv0, uv1);
    }
    ImGui::End();
}

void Game::Editor_CreateMainFrame()
{
    // Setup a main window with no frame and a dockspace that covers the entire viewport.
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    if (ImGui::Begin("Main Dock", nullptr, flags))
    {
        ImGuiID dockspaceID = ImGui::GetID("My Dockspace");
        ImGui::DockSpace(dockspaceID);
    }
    ImGui::End();
}

void Game::Editor_DisplayMainMenu()
{
    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Mario Kart"))
        {
            m_pCurrentScene = m_pScenes["Mariokart"];
        }

        if (ImGui::MenuItem("Play"))
        {
            m_GameState = GameState::Play;
        }

        if (ImGui::MenuItem("Stop"))
        {
            m_GameState = GameState::Stop;
        }

        if (ImGui::MenuItem("Save"))
        {
            SaveScene();
        }

        if (ImGui::MenuItem("Load"))
        {
            LoadScene();
        }
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}

void Game::Editor_DrawInspector()
{
    if (ImGui::Begin("Inspector"))
    {
        if (m_pSelectedGameObject)
        {
            m_pSelectedGameObject->AddToInspector();
        }

    }
    ImGui::End();

}

void Game::SaveScene()
{
    nlohmann::json json;

    m_pCurrentScene->SaveScene(json);

    std::string jsonString = json.dump(4);
    fw::SaveCompleteFile("Data/Scene/Test.scene", jsonString.c_str());
    
}

void Game::LoadScene()
{
    nlohmann::json json;

    std::string jsonString = fw::LoadCompleteFile("Data/Scene/Test.scene", nullptr);
    
    json = nlohmann::json::parse(jsonString);

    m_pCurrentScene->LoadScene(json);

    
}

void Game::Editor_DisplayObjectList()
{

    //ImGui::Begin( "Object List" );

    m_pCurrentScene->Editor_DisplayObjectList();

    //ImGui::End(); // "Object List"
}

