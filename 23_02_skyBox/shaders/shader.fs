#version 330 core
out vec4 FragColor;
float near = 0.1;
float far = 100.0;
float LinearizeDepth(float depth) {
float z_ndc = depth * 2.0 - 1.0; // back to NDC
return (2.0 * near * far) / (far + near - z_ndc  * (far - near));
}

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

struct Material { 
    sampler2D texture_diffuse1;
};

uniform Material material;
uniform sampler2D texture1;

uniform vec3 viewPos; 
uniform samplerCube skybox; 

void main() {
    //vec4 fragColor = texture(material.texture_diffuse1, TexCoords);
    //if (fragColor.a < 0.1)
     //   discard;
    // FragColor = fragColor;

    // FragColor = vec4(vec3(gl_FragCoord.z), 1.0f);

    // float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    // FragColor = vec4(vec3(depth), 1.0);
    
    // FragColor = texture(texture1, TexCoords);

    vec3 I = normalize(FragPos - viewPos); 
    float ratio = 1.00 / 2.42; 
    vec3 R = refract(I, normalize(Normal), ratio); 
    FragColor = vec4(texture(skybox, R).rgb, 1.0); 

    
}
