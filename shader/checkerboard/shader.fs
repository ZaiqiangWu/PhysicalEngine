#version 330 core
in vec3 VertexColor;
in vec2 TexCoord;
in float depth;
out vec4 pixel_color;

//uniform int GridNum;

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

pixel_color = img_color;
}
