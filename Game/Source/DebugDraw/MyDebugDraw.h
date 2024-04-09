#pragma once

#include "Framework.h"
#include "DataTypes.h"

class fw::Uniforms;
class fw::ShaderProgram;
struct VertexFormat_PosColor;


class MyDebugDraw : public b2Draw
{
public:


	virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {}
	virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
	virtual void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) {}
	virtual void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) {}
	virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {}
	/// @param xf a transform.
	virtual void DrawTransform(const b2Transform& xf) {}
	virtual void DrawPoint(const b2Vec2& p, float size, const b2Color& color) {}
	void DrawShape(VertexFormat_PosColor* triangleVerts, uint16* triangleIndices, uint32_t numVertices, uint32_t numIndices);

public:

	fw::Uniforms* m_pUniforms;
	fw::ShaderProgram* m_ShaderHandle;
	int m_ViewID;
	bool m_IsXY = true;
};
