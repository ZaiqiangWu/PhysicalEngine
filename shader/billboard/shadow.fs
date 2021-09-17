#version 330 core
in vec3 VertexColor;
in vec2 TexCoord;
out vec4 pixel_color;

uniform sampler2D ourTexture;
void main()
{
vec4 img_color=texture(ourTexture,TexCoord);
if(img_color.w<0.01)
{
	//gl_FragDepth = 1;
	discard;
}
pixel_color = img_color;
}
