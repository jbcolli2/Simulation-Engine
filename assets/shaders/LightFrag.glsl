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
    vec3 color;
    float diffuseIntensity;
    float specularIntensity;
};

struct PointLight
{
    vec3 position;
    vec3 color;
    float diffuseIntensity;
    float specularIntensity;
};

uniform DirLight dirLight;
uniform PointLight pointLight;
uniform float ambientIntensity;


uniform vec3 diffuseMat;
uniform uint roughness;

uniform vec3 cameraPosition;

// Helper function declaration
float specularAngleCoeff(vec3 light2Frag, vec3 cam2Frag, vec3 normal);
vec3 DirLightContrib(DirLight dirLight, vec3 cam2Frag, vec3 normal);
vec3 PointLightContrib(PointLight pointLight, vec3 light2Frag, vec3 frag2Cam, vec3 normal);



void main()
{
    // Initial clean up and setup
    vec3 normal = normalize(fragIn.Normal);
    vec3 cam2Frag = normalize(fragIn.FragPos - cameraPosition);



    // Compute the value of the fragment
    vec3 result = ambientIntensity*diffuseMat;

    result += DirLightContrib(dirLight, cam2Frag, normal);
    result += PointLightContrib(pointLight, fragIn.FragPos - pointLight.position, cam2Frag, normal);

    FragColor = vec4(result, 1.0);
}


float specularAngleCoeff(vec3 frag2Light, vec3 cam2Frag, vec3 normal)
{
    vec3 reflectLightOnFrag = normalize(reflect(frag2Light, normal));
    return  pow(max(dot(reflectLightOnFrag, cam2Frag), 0.0), roughness);
}




vec3 DirLightContrib(DirLight dirLight, vec3 cam2Frag, vec3 normal)
{
    vec3 frag2Light = -dirLight.direction;

    float phongAngleCoeff = max(dot(normal, frag2Light), 0.0);
    vec3 diffuseColor = phongAngleCoeff*dirLight.color*diffuseMat;

    float specCoeff = specularAngleCoeff(frag2Light, cam2Frag, normal);
    vec3 specularColor = specCoeff*dirLight.color;

    return dirLight.diffuseIntensity*diffuseColor + dirLight.specularIntensity*specularColor;
}


vec3 PointLightContrib(PointLight pointLight, vec3 light2Frag, vec3 cam2Frag, vec3 normal)
{
    light2Frag = normalize(light2Frag);
    float phongAngleCoeff = max(dot(normal, -light2Frag), 0.0);
    vec3 diffuseColor = phongAngleCoeff*pointLight.color*diffuseMat;

    float specCoeff = specularAngleCoeff(-light2Frag, cam2Frag, normal);
    vec3 specularColor = specCoeff*pointLight.color;

    return pointLight.diffuseIntensity*diffuseColor + pointLight.specularIntensity*specularColor;
}
