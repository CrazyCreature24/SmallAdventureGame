#include "VertexFormats.h"
#include "GameMesh.h"



GameMesh::GameMesh(const bgfx::VertexLayout& vertexFormat, const char* filename)
{
    std::vector<fw::vec3> normals;
    std::vector<fw::vec2> textcoords;
    std::vector<fw::vec3> positions;

    std::vector<std::tuple< fw::ivec3, fw::ivec3, fw::ivec3>> faces;

    char* buffer = fw::LoadCompleteFile(filename, nullptr);
    assert(buffer);

    char* nextToken = nullptr;
    char* line = strtok_s(buffer, "\n", &nextToken);
    while (line)
    {
        int bp = 1;
        float nx, ny, nz;
        if (sscanf(line, "vn %f %f %f", &nx, &ny, &nz) == 3)
        {
            normals.push_back(fw::vec3(nx, ny, nz));
        }


        float tx, ty;
        if (sscanf(line, "vt %f %f", &tx, &ty) == 2)
        {
            textcoords.push_back(fw::vec2(tx, ty));
        }


        float px, py, pz;
        if (sscanf(line, "v %f %f %f", &px, &py, &pz) == 3)
        {
            positions.push_back(fw::vec3(px, py, pz));
        }

        int x1, y1, z1, x2, y2, z2, x3, y3, z3;
        if (sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &x1, &y1, &z1, &x2, &y2, &z2, &x3, &y3, &z3) == 9)
        {
            faces.push_back({ fw::ivec3(x1, y1, z1), fw::ivec3(x2, y2, z2), fw::ivec3(x3, y3, z3) });
        }


        line = strtok_s(0, "\n", &nextToken);
    }

    for (int i = 0; i < faces.size(); i++)
    {
        m_Verts.push_back({ positions[(std::get<0>(faces[i])).x - 1], textcoords[(std::get<0>(faces[i])).y - 1] });
        m_Verts.push_back({ positions[(std::get<1>(faces[i])).x - 1], textcoords[(std::get<1>(faces[i])).y - 1] });
        m_Verts.push_back({ positions[(std::get<2>(faces[i])).x - 1], textcoords[(std::get<2>(faces[i])).y - 1] });

    }

    for (int i = 0; i < m_Verts.size(); i++)
    {
        m_Indicies.push_back(i);
    }

    Create(VertexFormat_PosUV::format, m_Verts.data(), sizeof(VertexFormat_PosUV) * m_Verts.size(), m_Indicies.data(), sizeof(uint16) * (int)(m_Indicies.size()));

    delete buffer;

    // Ignore this stuff below.
    // /////////////////////
    //int num1;
    //float num2;
    //char* str = "I am number 142, you are number 13.567";
    //sscanf(str, "%*s %*s %*s %d, %*s %*s %*s %f", &num1, &num2);

    ////Normals
    //float x, y, z;

    //char* str = "vn -1 0 0";
    //sscanf(str, "vn %f %f %f", &x, &y, &z);

    /*m_Normals;
    m_Textcoords;
    m_Positions;*/


    
}

GameMesh::~GameMesh()
{

}
