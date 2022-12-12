#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TextCoords;

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
struct Light{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light; 
uniform vec3 viewPos;



void main()
{
    
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(-lightDir, normalize(light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);
    if(theta > light.outerCutOff )
    {
        /// ambient
        vec3 ambient = light.ambient * vec3(texture(material.diffuse, TextCoords));
      
        /// diffuse
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TextCoords)));
        
        /// specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * (spec * (/*vec3(1.0f) -*/ vec3(texture(material.specular, TextCoords))));
        float distance    = length(light.position - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + 
                    light.quadratic * (distance * distance));
        // ambient  *= attenuation; 
        // diffuse  *= attenuation;
        // specular *= attenuation;

        diffuse  *= intensity;
        specular *= intensity;

        vec3 result = ambient + diffuse + specular;
        FragColor = vec4(result, 1.0);
    } else 
    {
        FragColor = vec4(light.ambient * vec3(texture(material.diffuse, TextCoords)), 1.0f);
    }
    
}
