#version 330 core


in vertexData
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} fragIn;



out vec4 FragColor;

uniform sampler2D screenTex;

void main()
{
    vec3 hdrColor = texture(screenTex, fragIn.TexCoord).rgb;

    float exposure = 1.0;
    vec3 ldrColor = 1.0 - exp(-hdrColor*exposure);
//    ldrColor = pow(ldrColor, vec3(1.0/2.2));

    if(fragIn.FragPos.x < 0)
        FragColor = vec4(hdrColor, 1.0);
    else
        FragColor = vec4(ldrColor, 1.0);
}