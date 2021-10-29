#version 330 core

in vec3 ex_FragPos;
in vec3 normal;


out vec4 color;


uniform vec3 lightPos;


uniform vec3 objectColor;



void main()
{
     // Ambient
    //float ambientStrength = 0.2f;
    float ambient = 0.2;




    // Diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - ex_FragPos);
    float diffuse = max(dot(norm, lightDir)*1.5, 0.0);



    vec3 result = (ambient + diffuse)* objectColor;
    float gamma = 1.2;
    result = pow(result, vec3(1.0/gamma));
    color = vec4(result, 1);
    //color=vec4(1,0,0,1);
}