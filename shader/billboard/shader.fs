#version 330 core
in vec3 VertexColor;
in vec2 TexCoord;
out vec4 pixel_color;

uniform sampler2D ourTexture;
void main()
{
vec4 img_color=texture(ourTexture,TexCoord);
if(img_color.r+img_color.g+img_color.b>0.99*3)
discard;
pixel_color = img_color;
}
