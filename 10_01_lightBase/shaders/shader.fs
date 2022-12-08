#version 330 core
out vec4 FragColor;

//in vec2 TexCoord;

//uniform sampler2D texture0;
//uniform sampler2D textureSmile;
uniform float ratio;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    //FragColor = mix(texture(texture0, TexCoord), texture(textureSmile, vec2(1 - TexCoord.x, TexCoord.y)), ratio);
    
    FragColor = vec4(lightColor * objectColor, 1.0f);
}
