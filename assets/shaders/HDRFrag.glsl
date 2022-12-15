#version 330 core

in vec2 UV;

out vec4 FragColor;

uniform sampler2D quadTexture;
uniform float exposure;


void main()
{

    vec3 hdrColor = texture(quadTexture, UV).rgb;
    vec3 ldrColor = 1.0 - exp(-hdrColor * exposure);
    FragColor = vec4(ldrColor, 1);
}




