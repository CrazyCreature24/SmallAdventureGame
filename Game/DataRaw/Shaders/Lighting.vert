$input a_position, a_texcoord0, a_normal
$output v_texcoord0, v_normal, v_worldSpaceSurfacePos

#include <bgfx_shader.sh>

uniform vec4 u_UVScale;
uniform vec4 u_UVOffset;
uniform mat4 u_WorldMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjMatrix;
uniform mat4 u_NormalMatrix;

void main()
{
	vec4 objectSpacePosition = vec4(a_position, 1);
	vec4 worldSpacePosition = mul(u_WorldMatrix, objectSpacePosition);
	vec4 viewSpacePosition = mul(u_ViewMatrix, worldSpacePosition);
	vec4 clipSpacePosition = mul(u_ProjMatrix, viewSpacePosition);
	gl_Position = clipSpacePosition;

	v_texcoord0 = a_texcoord0 * u_UVScale.xy + u_UVOffset.xy;

	v_normal = mul(u_NormalMatrix, vec4(a_normal, 0)).xyz;

	v_worldSpaceSurfacePos = worldSpacePosition.xyz;
}
