#version 330 core
in vec3 VertexColor;
in vec2 TexCoord;
out vec4 pixel_color;

//uniform int GridNum;

void main()
{
int GridNum=50;
vec4 img_color=vec4(1,1,1,1);

if(int(TexCoord.x*GridNum)%2==0)
{
	if(int(TexCoord.y*GridNum)%2==0)
	{
		img_color=vec4(1,1,1,1);
	}
	else
	{
		img_color=vec4(0,0,0,1);
	}
}
else
{
	if(int(TexCoord.y*GridNum)%2==0)
	{
		img_color=vec4(0,0,0,1);
	}
	else
	{
		img_color=vec4(1,1,1,1);
	}
}

pixel_color = img_color;
}
