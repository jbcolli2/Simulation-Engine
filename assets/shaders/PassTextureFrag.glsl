#version 330 core

in vec2 UV;

out vec4 FragColor;

uniform sampler2D quadTexture;
uniform sampler2D trash;


void main()
{

    vec3 result = texture(quadTexture, UV).rgb;
    FragColor = vec4(result, 1);
}




