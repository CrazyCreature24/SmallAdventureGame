$input v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D( u_TextureColor, 0 );

void main()
{
    vec4 color = texture2D( u_TextureColor, v_texcoord0 );

    //color.rgb += vec3( 0.1, 0.3, 0.7 );
    //color.rgb *= vec3( 0.1, 0.3, 0.7 );

    vec3 color1 = vec3( 0.1, 0.3, 0.7 ); // Pass these in as uniforms
    vec3 color2 = vec3( 0.7, 0.3, 0.1 );

    float perc = color.x;
    color.rgb = mix(color1, color2, perc);

    gl_FragColor = color;



}
