//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Framework.h"

#include "VertexFormats.h"
#include "Shapes.h"

//=======================
// Triangle
//=======================
static const VertexFormat_PosColor g_TriangleVerts[] =
{
    { vec2( 0.0f, 0.5f), 255, 0, 0, 255 }, // Top
    { vec2(-0.5f,-0.5f), 0, 255, 0, 255 }, // Bottom-left
    { vec2( 0.5f,-0.5f), 0, 0, 255, 255 }, // Bottom-right
};
static const uint16 g_TriangleIndices[] =
{
    0, 2, 1,
};
fw::Mesh* CreateTriangleMesh()
{
    return new fw::Mesh( VertexFormat_PosColor::format, g_TriangleVerts, sizeof(g_TriangleVerts), g_TriangleIndices, sizeof(g_TriangleIndices) );
}

//=======================
// Square
//=======================
static const VertexFormat_PosColor g_SquareVerts[] =
{
    { vec2(-0.5f, 0.5f), 0, 255, 0, 255 }, // Top-left
    { vec2(-0.5f,-0.5f), 0,   0, 0, 255 }, // Bottom-left
    { vec2( 0.5f,-0.5f), 0,   0, 0, 255 }, // Bottom-right

    { vec2(-0.5f, 0.5f), 0, 255, 0, 255 }, // Top-left
    { vec2( 0.5f,-0.5f), 0,   0, 0, 255 }, // Bottom-right
    { vec2( 0.5f, 0.5f), 0, 255, 0, 255 }, // Top-right
};
static const uint16 g_SquareIndices[] =
{
    0,2,1, 3,5,4
};
fw::Mesh* CreateSquareMesh()
{
    return new fw::Mesh( VertexFormat_PosColor::format, g_SquareVerts, sizeof(g_SquareVerts), g_SquareIndices, sizeof(g_SquareIndices) );
}

//=======================
// Sprite
//=======================
static const VertexFormat_PosUV g_SpriteVerts[] =
{
    { vec2(-0.5f, 0.5f), vec2(0,1) }, // Top-left
    { vec2(-0.5f,-0.5f), vec2(0,0) }, // Bottom-left
    { vec2( 0.5f,-0.5f), vec2(1,0) }, // Bottom-right

    { vec2(-0.5f, 0.5f), vec2(0,1) }, // Top-left
    { vec2( 0.5f,-0.5f), vec2(1,0) }, // Bottom-right
    { vec2( 0.5f, 0.5f), vec2(1,1) }, // Top-right
};
static const uint16 g_SpriteIndices[] =
{
    0,2,1, 3,5,4
};
fw::Mesh* CreateSpriteMesh()
{
    return new fw::Mesh( VertexFormat_PosUV::format, g_SpriteVerts, sizeof(g_SpriteVerts), g_SpriteIndices, sizeof(g_SpriteIndices) );
}

//=======================
// Double Sided Sprite
//=======================
static const VertexFormat_PosUV g_KartSpriteVerts[] =
{
    //Front
    { vec3(-0.5f, 0.5f), vec2(0,1) }, // Top-left
    { vec3(-0.5f,-0.5f), vec2(0,0) }, // Bottom-left
    { vec3(0.5f,-0.5f), vec2(0.5f,0) }, // Bottom-right

    { vec3(-0.5f, 0.5f), vec2(0,1) }, // Top-left
    { vec3(0.5f,-0.5f), vec2(0.5f,0) }, // Bottom-right
    { vec3(0.5f, 0.5f), vec2(0.5f,1) }, // Top-right

    //Back
    { vec3(-0.5f, 0.5f), vec2(0.5f,1) }, // Top-left
    { vec3(-0.5f,-0.5f), vec2(0.5f,0) }, // Bottom-left
    { vec3(0.5f,-0.5f), vec2(1,0) }, // Bottom-right

    { vec3(-0.5f, 0.5f), vec2(0.5f,1) }, // Top-left
    { vec3(0.5f,-0.5f), vec2(1,0) }, // Bottom-right
    { vec3(0.5f, 0.5f), vec2(1,1) }, // Top-right
};
static const uint16 g_KartSpriteIndices[] =
{
    0,1,2, 3,4,5, 
    6,8,7, 9,11,10
};
fw::Mesh* CreateDoubleSidedSpriteMesh()
{
    return new fw::Mesh(VertexFormat_PosUV::format, g_KartSpriteVerts, sizeof(g_KartSpriteVerts), g_KartSpriteIndices, sizeof(g_KartSpriteIndices));
}


//=======================
// 3D Sprite
//=======================
static const VertexFormat_PosUV3D g_3DSpriteVerts[] =
{
    //Front
    { vec3(-0.5f, 0.5f, -0.5f), vec2(0, 1) }, // Top-left
    { vec3(-0.5f,-0.5f, -0.5f), vec2(0, 0) }, // Bottom-left
    { vec3(0.5f,-0.5f, -0.5f), vec2((float)1/6, 0)}, // Bottom-right
    { vec3(0.5f, 0.5f, -0.5f), vec2((float)1/6, 1)}, // Top-right

    //Left
    { vec3(-0.5f, 0.5f, 0.5f), vec2((float)1 / 6,1) }, // Top-left
    { vec3(-0.5f,-0.5f, 0.5f), vec2((float)1 / 6,0) }, // Bottom-left
    { vec3(-0.5f,-0.5f, -0.5f), vec2((float)2 / 6,0) }, // Bottom-right
    { vec3(-0.5f, 0.5f, -0.5f), vec2((float)2 / 6,1) }, // Top-right

    //Right
    { vec3(-0.5f, 0.5f, 0.5f), vec2((float)2 / 6,1) }, // Top-left
    { vec3(-0.5f,-0.5f, 0.5f), vec2((float)2 / 6,0) }, // Bottom-left
    { vec3(0.5f,-0.5f, 0.5f), vec2((float)3 / 6,0) }, // Bottom-right
    { vec3(0.5f, 0.5f, 0.5f), vec2((float)3 / 6,1) }, // Top-right

    //Back
    { vec3(0.5f, 0.5f, -0.5f), vec2((float)3 / 6,1) }, // Top-left
    { vec3(0.5f,-0.5f, -0.5f), vec2((float)3 / 6,0) }, // Bottom-left
    { vec3(0.5f,-0.5f, 0.5f), vec2((float)4 / 6,0) }, // Bottom-right
    { vec3(0.5f, 0.5f, 0.5f), vec2((float)4 / 6,1) }, // Top-right

    //Top
    { vec3(-0.5f, 0.5f, 0.5f), vec2((float)4 / 6,1) }, // Top-left
    { vec3(-0.5f, 0.5f, -0.5f), vec2((float)4 / 6,0) }, // Bottom-left
    { vec3(0.5f, 0.5f, -0.5f), vec2((float)5 / 6,0) }, // Bottom-right
    { vec3(0.5f, 0.5f, 0.5f), vec2((float)5 / 6,1) }, // Top-right

    //Bottom
    { vec3(0.5f, -0.5f, -0.5f), vec2((float)5 / 6,1) }, // Top-left
    { vec3(0.5f,-0.5f, 0.5f), vec2((float)5 / 6,0) }, // Bottom-left
    { vec3(-0.5f,-0.5f, 0.5f), vec2(1,0) }, // Bottom-right
    { vec3(-0.5f, -0.5f, -0.5f), vec2(1,1) }, // Top-right
};
static const uint16 g_3DSpriteIndices[] =
{
    0,2,1, 2,0,3,
    4,6,5, 6,4,7,
    8,9,10, 10,11,8,
    12,14,13, 14,12,15,
    16,18,17, 18,16,19,
    20,21,22, 22,23,20
};

fw::Mesh* Create3dSpriteMesh(vec3 size)
{
    VertexFormat_PosUV3D SpriteVerts3D[24];

    for (int i = 0; i < 24; i++)
    {
        SpriteVerts3D[i] = g_3DSpriteVerts[i];
    }

    for (auto& i : SpriteVerts3D)
    {
        i.pos *= size;
    }

    return new fw::Mesh(VertexFormat_PosUV3D::format, SpriteVerts3D, sizeof(SpriteVerts3D), g_3DSpriteIndices, sizeof(g_3DSpriteIndices));
}


//=======================
// Sprite Normal
//=======================
static const VertexFormat_PosUVNormal g_SpriteNormalVerts[] =
{
    { vec2(-0.5f, 0.5f), vec2(0,1), vec3(0,0,-1) }, // Top-left
    { vec2(-0.5f,-0.5f), vec2(0,0), vec3(0,0,-1) }, // Bottom-left
    { vec2( 0.5f,-0.5f), vec2(1,0), vec3(0,0,-1) }, // Bottom-right

    { vec2(-0.5f, 0.5f), vec2(0,1), vec3(0,0,-1) }, // Top-left
    { vec2( 0.5f,-0.5f), vec2(1,0), vec3(0,0,-1) }, // Bottom-right
    { vec2( 0.5f, 0.5f), vec2(1,1), vec3(0,0,-1) }, // Top-right
};
static const uint16 g_SpriteNormalIndices[] =
{
    0,2,1, 3,5,4
};
fw::Mesh* CreateSpriteNormalMesh()
{
    return new fw::Mesh(VertexFormat_PosUVNormal::format, g_SpriteNormalVerts, sizeof(g_SpriteNormalVerts), g_SpriteNormalIndices, sizeof(g_SpriteNormalIndices));
}