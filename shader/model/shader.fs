#version 330 core

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;


out vec4 color;


uniform sampler2D texture_diffuse1;

uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform bool TextureValid;
uniform vec3 Ambient;
uniform vec3 Diffuse;
uniform vec3 Specular;


void main()
{    
    //vec3 lightPos=vec3(1,1,1);
     //vec3 lightColor=vec3(1,1,1);


     // Ambient
    //float ambientStrength = 0.2f;
    vec3 ambient = Ambient * lightColor;
    vec4 tex;
    vec3 objectColor;
    
    tex=texture(texture_diffuse1, TexCoords);
    objectColor = vec3(tex);
    if(!TextureValid)
    objectColor = vec3(0.4,0.4,0.4);
    
    
    

    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor*Diffuse;

    // Specular
    //float specularStrength = 0.2f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = Specular * spec * lightColor;  

    vec3 result = (ambient + diffuse+specular)* objectColor;
    float gamma = 1.2;
    result = pow(result, vec3(1.0/gamma));
    color = vec4(result, tex.w);
}