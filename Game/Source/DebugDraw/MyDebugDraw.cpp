#include "Meshes/VertexFormats.h"
#include "MyDebugDraw.h"

void MyDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	VertexFormat_PosColor verts[4];

	for (int i = 0; i < 4; i++)
	{
		if (m_IsXY)
		{
			verts[i].pos.Set(vertices[i].x, vertices[i].y, 0);
		}
		else
		{
			verts[i].pos.Set(vertices[i].x, 0, vertices[i].y);
		}
		verts[i].color[0] = (uint8)(color.r * 255);
		verts[i].color[1] = (uint8)(color.g * 255);
		verts[i].color[2] = (uint8)(color.b * 255);
		verts[i].color[3] = 100; // (uint16)(color.b * 255);
	}

	uint16 indicies[6] =
	{
		1,0,2, 0,3,2
	};

	DrawShape(verts, indicies, 4, 6);
}

void MyDebugDraw::DrawShape(VertexFormat_PosColor* triangleVerts, uint16* triangleIndices, uint32_t numVertices, uint32_t numIndices)
{
	bgfx::TransientVertexBuffer tvb;
	bgfx::TransientIndexBuffer tib;

	int maxVerts = bgfx::getAvailTransientVertexBuffer(numVertices, VertexFormat_PosColor::format);
	int maxIndices = bgfx::getAvailTransientIndexBuffer(numIndices);
	if ((numVertices != maxVerts) || (numIndices != maxIndices))
	{
		// Not enough space in transient buffer, quit drawing the rest...
		return;
	}

	mat4 worldMatrix;
	worldMatrix.SetIdentity();
	bgfx::setUniform(m_pUniforms->GetUniform("u_WorldMatrix"), &worldMatrix);

	bgfx::allocTransientVertexBuffer(&tvb, numVertices, VertexFormat_PosColor::format);
	bgfx::allocTransientIndexBuffer(&tib, numIndices);

	VertexFormat_PosColor* vertData = (VertexFormat_PosColor*)tvb.data;
	memcpy(vertData, triangleVerts, numVertices * sizeof(VertexFormat_PosColor::format));

	uint16* indexData = (uint16*)tib.data;
	memcpy(indexData, triangleIndices, numIndices * sizeof(uint16));

	uint64_t state = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_MSAA |
		BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);


	bgfx::setState(state);
	bgfx::setVertexBuffer(0, &tvb, 0, numVertices);
	bgfx::setIndexBuffer(&tib, 0, numIndices);
	bgfx::submit(m_ViewID, m_ShaderHandle->GetProgram());
}