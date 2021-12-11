#version 330 core
in vec3 VertexColor;
in vec2 TexCoord;
in float depth;
out vec4 pixel_color;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D shadowMap;


float ShadowCalculation(vec4 fragPosLightSpace)
{
    // 执行透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 变换到[0,1]的范围
    projCoords = projCoords * 0.5 + 0.5;
    // 取得最近点的深度(使用[0,1]范围下的fragPosLight当坐标)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // 取得当前片元在光源视角下的深度
    float currentDepth = projCoords.z;
    // 检查当前片元是否在阴影中

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    int k=0;
    for(int x = -k; x <= k; ++x)
    {
        for(int y = -k; y <= k; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth  > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= (k+1)*(k+1);
    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

void GetCoef(in int GridNum,in float t,inout float coef)
{
    float sm=min(0.002*pow(1.09,depth),0.15);
    float tt=t*GridNum;
    int dt=int(tt);
    if(dt%2==0)
    {
        coef=1.0;
    }
    else
    {
        coef=-1.0;
    }
    float delta=min(ceil(tt)-tt,tt-floor(tt));
    if(delta<sm)
    {
        coef=coef*(delta*(0.5/sm)+0.5);
    }

}

void main()
{
int GridNum=50;
vec4 img_color=vec4(1,1,1,1);
vec4 color_a=vec4(1,1,1,1);
vec4 color_b=vec4(0.4,0.4,0.4,1);
float coef_a;
float coef_b;

GetCoef(GridNum,TexCoord.x,coef_a);
GetCoef(GridNum,TexCoord.y,coef_b);

float ss=max(0.5,abs(coef_a*coef_b));
if(coef_a*coef_b>0)
{


    img_color=color_a*ss+color_b*(1-ss);
}
else
{
    img_color=color_b*ss+color_a*(1-ss);
}


float shadow = ShadowCalculation(fs_in.FragPosLightSpace);

pixel_color = vec4(img_color.xyz*(min(1.5-shadow,1)),img_color.w);
//pixel_color = vec4(vec3(texture(shadowMap, TexCoord).r),1);
}
