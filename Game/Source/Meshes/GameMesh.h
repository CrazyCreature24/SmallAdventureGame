#pragma once
#include "Framework.h"


class GameMesh : public fw::Mesh 
{
public:

    GameMesh(const bgfx::VertexLayout& vertexFormat, const char* filename);
    ~GameMesh();

protected:

    std::vector<VertexFormat_PosUV> m_Verts;
    std::vector<uint16> m_Indicies;

    
};