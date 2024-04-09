//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"

#include "ResourceManager.h"
#include "Renderer/Material.h"
#include "Renderer/Mesh.h"
#include "Renderer/ShaderProgram.h"
#include "Renderer/Texture.h"

namespace fw {

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
    for( auto& meshPair : m_Meshes )
    {
        delete meshPair.second;
    }

    for( auto& materialPair : m_Materials )
    {
        delete materialPair.second;
    }

    for( auto& texturePair : m_Textures )
    {
        delete texturePair.second;
    }

    for( auto& shaderPair : m_Shaders )
    {
        delete shaderPair.second;
    }
}

void ResourceManager::AddMesh(std::string name, Mesh* pMesh)
{
    if( m_Meshes.find(name) == m_Meshes.end() )
    {
        m_Meshes[name] = pMesh;
    }
}

void ResourceManager::AddShader(std::string name, ShaderProgram* pMesh)
{
    if( m_Shaders.find(name) == m_Shaders.end() )
    {
        m_Shaders[name] = pMesh;
    }
}

void ResourceManager::AddTexture(std::string name, Texture* pMesh)
{
    if( m_Textures.find(name) == m_Textures.end() )
    {
        m_Textures[name] = pMesh;
    }
}

void ResourceManager::AddMaterial(std::string name, Material* pMesh)
{
    if( m_Materials.find(name) == m_Materials.end() )
    {
        m_Materials[name] = pMesh;
    }
}

Mesh* ResourceManager::GetMesh(std::string name)
{
    if( m_Meshes.find(name) != m_Meshes.end() )
    {
        return m_Meshes[name];
    }

    return nullptr;
}

ShaderProgram* ResourceManager::GetShader(std::string name)
{
    if(m_Shaders.find(name) != m_Shaders.end() )
    {
        return m_Shaders[name];
    }

    return nullptr;
}

Texture* ResourceManager::GetTexture(std::string name)
{
    if( m_Textures.find(name) != m_Textures.end() )
    {
        return m_Textures[name];
    }

    return nullptr;
}

Material* ResourceManager::GetMaterial(std::string name)
{
    if( m_Materials.find(name) != m_Materials.end() )
    {
        return m_Materials[name];
    }

    return nullptr;
}

std::string ResourceManager::GetMeshName(fw::Mesh* pMesh)
{
    for (auto i : m_Meshes)
    {
        if (i.second == pMesh)
        {
            return i.first;
        }
    }
    assert(false && "Mesh not found");

    return "Mesh Not Found";
}

std::string ResourceManager::GetMaterialName(fw::Material* pMaterial)
{
    for (auto i : m_Materials)
    {
        if (i.second == pMaterial)
        {
            return i.first;
        }
    }
    assert(false && "Material not found");

    return "Material not found";
}

} // namespace fw
