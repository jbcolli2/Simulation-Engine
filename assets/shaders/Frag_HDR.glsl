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
    FragColor = texture(screenTex, fragIn.TexCoord);
}