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





uniform mat4 model;
layout (std140) uniform VP
{
    mat4 view;
    mat4 proj;
};


void main()
{
    gl_Position = proj*view*model*vec4(aPosition, 1.0f);
//    gl_Position = vec4(aPosition, 1.0f);

    vertexOut.FragPos = vec3(model*vec4(aPosition, 1.0));
    mat3 invTransModel = mat3(inverse(transpose(model)));
    vertexOut.Normal = invTransModel*aNormal;
    vertexOut.TexCoord = aUV;
}
