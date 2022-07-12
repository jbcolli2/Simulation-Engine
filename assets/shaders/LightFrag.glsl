#version 330 core

in vertexData
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} fragIn;

out vec4 FragColor;

struct DirLight
{
    vec3 direction;
    vec3 diffuse;
    float ambientFactor;
    vec3 specular;
};

uniform DirLight dirLight;
uniform vec3 camPosition;

// Helper function declaration
vec3 DirLightContrib(DirLight dirLight, vec3 fragPos, vec3 normal);



void main()
{
    // Initial clean up and setup
    vec3 normal = normalize(fragIn.Normal);


    // Compute the value of the fragment
    vec3 result = vec3(0);

    result += DirLightContrib(dirLight, fragIn.FragPos, normal);

    FragColor = vec4(result, 1.0);
}




vec3 DirLightContrib(DirLight dirLight, vec3 fragPos, vec3 normal)
{
    vec3 frag2Light = -dirLight.direction;

    vec3 ambient = dirLight.ambientFactor*dirLight.diffuse;
    vec3 diffuse = dot(normal, frag2Light)*dirLight.diffuse;

    return ambient + diffuse;
}