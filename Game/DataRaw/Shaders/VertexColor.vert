$input a_position, a_color0
$output v_color0


#include <bgfx_shader.sh>


uniform mat4 u_WorldMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjMatrix;

void main()
{
	
	vec4 objectSpacePosition = vec4(a_position, 1);
	vec4 worldSpacePosition = mul(u_WorldMatrix, objectSpacePosition);
	vec4 viewSpacePosition = mul(u_ViewMatrix, worldSpacePosition);
	vec4 clipSpacePosition = mul(u_ProjMatrix, viewSpacePosition);
	gl_Position = clipSpacePosition;

    v_color0 = a_color0;
}


