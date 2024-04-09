$input v_texcoord0, v_normal

#include <bgfx_shader.sh>

SAMPLER2D( u_TextureColor, 0 );

void main()
{
    vec4 color = texture2D( u_TextureColor, v_texcoord0 );
    gl_FragColor = color;

    gl_FragColor = vec4(v_normal,1);
}
