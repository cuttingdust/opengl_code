#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TextCoords;

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};
struct Light{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light; 
uniform vec3 viewPos;

void main()
{
    /// ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TextCoords));
      
    /// diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TextCoords)));
    
    /// specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * (/*vec3(1.0f) -*/ vec3(texture(material.specular, TextCoords))));

    /// emission
    vec3 emission = vec3(texture(material.emission, TextCoords));

    vec3 result = ambient + diffuse + specular + emission;
    FragColor = vec4(result, 1.0);
}
