//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include <map>
#include <string>

namespace fw {

class Mesh;
class ShaderProgram;
class Texture;
class Material;

class ResourceManager
{
public:
    ResourceManager();
    ~ResourceManager();

    void AddMesh(std::string name, Mesh* pMesh);
    void AddShader(std::string name, ShaderProgram* pMesh);
    void AddTexture(std::string name, Texture* pMesh);
    void AddMaterial(std::string name, Material* pMesh);

    Mesh* GetMesh(std::string name);
    ShaderProgram* GetShader(std::string name);
    Texture* GetTexture(std::string name);
    Material* GetMaterial(std::string name);

    std::string GetMeshName(fw::Mesh* pMesh);
    std::string GetMaterialName(fw::Material* pMaterial);

protected:
    std::map<std::string, fw::Mesh*> m_Meshes;
    std::map<std::string, fw::ShaderProgram*> m_Shaders;
    std::map<std::string, fw::Texture*> m_Textures;
    std::map<std::string, fw::Material*> m_Materials;
};

} // namespace fw
