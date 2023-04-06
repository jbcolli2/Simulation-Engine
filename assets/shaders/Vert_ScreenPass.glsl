#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

out vertexData
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} vertexOut;







void main()
{
    gl_Position = vec4(aPosition, 1.0f);

    vertexOut.FragPos = vec3(gl_Position);
    vertexOut.TexCoord = aUV;
}
