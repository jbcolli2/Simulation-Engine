#version 330 core

in vertexData
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} fragIn;

out vec4 FragColor;

// Basic Scene uniforms
uniform float ambientIntensity;
uniform vec3 cameraPosition;

//Flags
uniform bool useSolidMaterial;
uniform bool useTextureMaterial;
uniform bool useDirLight;
uniform bool usePointLight;


float specularAngleCoeff(vec3 frag2Light, vec3 cam2Frag, vec3 normal, float roughness)
{
    vec3 reflectLightOnFrag = normalize(reflect(frag2Light, normal));
    return  pow(max(dot(reflectLightOnFrag, cam2Frag), 0.0), roughness);
}




struct SolidMaterial
{
    vec3 diffuse;
    vec3 specular;
    float roughness;
};
uniform SolidMaterial solidMat;


struct TextureMaterial
{
    sampler2D diffuse;
    vec3 specular;
    float roughness;
};
uniform TextureMaterial textureMat;




struct DirLight
{
    vec3 direction;
    vec3 color;
    float diffIntensity;
    float specIntensity;
};
uniform DirLight dirLight;


vec3 DirLightContrib(DirLight dirLight, vec3 cam2Frag, vec3 normal, vec3 diff, vec3 spec, float rough);





struct PointLight
{
    vec3 position;
    vec3 color;
    float diffIntensity;
    float specIntensity;
};
uniform PointLight pointLight;


vec3 PointLightContrib(PointLight pointLight, vec3 light2Frag, vec3 cam2Frag, vec3 normal,
            vec3 diff, vec3 spec, float rough);






// Helper function declaration
//float specularAngleCoeff(vec3 light2Frag, vec3 cam2Frag, vec3 normal);
//vec3 DirLightContrib(DirLight dirLight, vec3 cam2Frag, vec3 normal);
//vec3 PointLightContrib(PointLight pointLight, vec3 light2Frag, vec3 frag2Cam, vec3 normal);



void main()
{
    // Initial clean up and setup
    vec3 normal = normalize(fragIn.Normal);
    vec3 cam2Frag = normalize(fragIn.FragPos - cameraPosition);

    vec3 diff, spec;
    float rough;

    if(useSolidMaterial)
    {
        diff = solidMat.diffuse;
        spec = solidMat.specular;
        rough = solidMat.roughness;
    }
    if(useTextureMaterial)
    {
        diff = vec3(texture(textureMat.diffuse, fragIn.TexCoord));
        spec = textureMat.specular;
        rough = textureMat.roughness;
    }

    // Compute the value of the fragment
    vec3 result = ambientIntensity*solidMat.diffuse;

    if(useDirLight)
    {
        result += DirLightContrib(dirLight, cam2Frag, normal, diff, spec, rough);
    }
    if(usePointLight)
    {
        result += PointLightContrib(pointLight, fragIn.FragPos - pointLight.position, cam2Frag, normal, diff, spec, rough);
    }

    FragColor = vec4(result, 1.0);
}



vec3 DirLightContrib(DirLight dirLight, vec3 cam2Frag, vec3 normal, vec3 diff, vec3 spec, float rough)
{
    vec3 frag2Light = -dirLight.direction;

    float phongAngleCoeff = max(dot(normal, frag2Light), 0.0);
    vec3 diffuseColor = phongAngleCoeff*dirLight.color*diff;

    float specCoeff = specularAngleCoeff(frag2Light, cam2Frag, normal, rough);
    vec3 specularColor = specCoeff*dirLight.color*spec;

    return dirLight.diffIntensity*diffuseColor + dirLight.specIntensity*specularColor;
}




vec3 PointLightContrib(PointLight pointLight, vec3 light2Frag, vec3 cam2Frag, vec3 normal, vec3 diff, vec3 spec, float rough)
{

    light2Frag = normalize(light2Frag);
    float phongAngleCoeff = max(dot(normal, -light2Frag), 0.0);
    vec3 diffuseColor = phongAngleCoeff*pointLight.color*diff;

    float specCoeff = specularAngleCoeff(-light2Frag, cam2Frag, normal, rough);
    vec3 specularColor = specCoeff*pointLight.color*spec;

    return pointLight.diffIntensity*diffuseColor + pointLight.specIntensity*specularColor;
}

