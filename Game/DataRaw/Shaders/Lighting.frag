$input v_texcoord0, v_normal, v_worldSpaceSurfacePos

#include <bgfx_shader.sh>

SAMPLER2D( u_TextureColor, 0 );

uniform vec4 u_CameraPosition;
uniform vec4 u_LightPosition[8];
uniform vec4 u_LightColor[8];
uniform vec4 u_LightRange[8];


vec4 SetupLight(vec4 color, vec3 normal, vec3 wps, int index);

float GetLightIntensity(vec3 directionToLight, int index);

float CalcDiffuse(vec3 directionToLight, vec3 normal);

void main()
{
    vec4 matColor = texture2D( u_TextureColor, v_texcoord0 );
    vec4 color = vec4(0,0,0,1); 

    for (int i = 0; i < 8; i++)
    {
        color += SetupLight(matColor, v_normal, v_worldSpaceSurfacePos, i);
    }

    vec3 outColor = v_normal;
    gl_FragColor = color; //vec4(outColor,1.0);

}

vec4 SetupLight(vec4 color, vec3 normal, vec3 wps, int index)
{
    vec3 lightPosition = u_LightPosition[index].xyz; //vec3(20,50,0);
    vec3 surfacePosition = wps;
    vec3 lightColor =  u_LightColor[index].xyz; //vec3(1,0,1);

    vec3 directionToLightNotNormalize = lightPosition - surfacePosition;
    vec3 directionToLight = normalize(directionToLightNotNormalize);

    // Diffuse Percentage
    float diffusePercentage = CalcDiffuse(directionToLight, normal);
    
    //Specular Percentage (Reflection perc)
    float shine = 200.0; //This should be 200
    vec3 cameraPos = u_CameraPosition.xyz;
    vec3 directionToCam = normalize(cameraPos - surfacePosition);
    vec3 halfVector = normalize(directionToLight + directionToCam);
    float specularPerc = pow(max(0,dot(normal, halfVector)), shine);

    //Light intensity
    float lightIntensity = GetLightIntensity(directionToLightNotNormalize, index);

    vec4 finalColor = vec4(color.rgb * lightColor * diffusePercentage, 1);
    finalColor += vec4(lightColor * specularPerc, 0);
    vec4 endColor = vec4(finalColor.xyz * lightIntensity, color.a);

    return endColor;
}

float GetLightIntensity(vec3 directionToLight, int index) 
{
    float Length = length(directionToLight);
    float lightRange = u_LightRange[index].x; //20.0;
    float exponent = 3.7;
    float lightIntensity = pow(max(0.0, 1.0 - (Length / lightRange)), exponent);
    return lightIntensity;
}

float CalcDiffuse(vec3 directionToLight, vec3 normal)
{
    float diffusePercentage;
    diffusePercentage = max(0,dot(directionToLight, normal));

    if (diffusePercentage < 0)
    {
        diffusePercentage = 0;
    }
    return diffusePercentage;
}