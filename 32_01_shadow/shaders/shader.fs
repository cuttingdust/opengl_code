#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture; 
uniform sampler2D shadowMap; 
uniform vec3 lightPos; 
uniform vec3 viewPos; 

float ShadowCalculation(vec4 fragPosLightSpace) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5f + 0.5f;
    if (projCoords.z > 1.0f) {
        return 0.0f;
    }
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = max(0.05f * (1.0f - dot(fs_in.Normal, lightPos - fs_in.FragPos)), 0.005f);
    float shadow = 0.0f;
    // shadow = currentDepth - bias > closestDepth? 1.0f: 0.0f;
    vec2 texelSize = 1.0f / textureSize(shadowMap, 0); 
    for(int x = -1; x <= 1; ++x) { 
		for(int y = -1; y <= 1; ++y) { 
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
		    shadow += currentDepth - bias > pcfDepth? 1.0f: 0.0f; 
	    } 
    }
    shadow /= 9.0;

    return shadow;
}

void main()
{
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb; 
    vec3 normal = normalize(fs_in.Normal); 

    /// ambient
    vec3 lightColor = vec3(1.0); 
    vec3 ambient = 0.15 * lightColor;

    /// diffuse 
    vec3 lightDir = normalize(lightPos - fs_in.FragPos); 
	float diff = max(dot(lightDir, normal), 0.0); 
	vec3 diffuse = diff * lightColor; 

    /// specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos); 
	float spec = 0.0; 
	vec3 halfwayDir = normalize(lightDir + viewDir); 
	spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0); 
	vec3 specular = spec * lightColor;

    /// calculate shadow 
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace); 
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color; 
	FragColor = vec4(lighting, 1.0); 
}
