#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in vec3 FragPos [];
out vec3 normal;
out vec3 ex_FragPos;

void main( void )
{
    //vec3 a = ( gl_in[1].gl_Position - gl_in[0].gl_Position ).xyz;
    vec3 a = FragPos[1]-FragPos[0];
    //vec3 b = ( gl_in[2].gl_Position - gl_in[0].gl_Position ).xyz;
    vec3 b = FragPos[2]-FragPos[0];
    vec3 N = -normalize( cross( b, a ) );

    for( int i=0; i<gl_in.length( ); ++i )
    {
        gl_Position = gl_in[i].gl_Position;
        normal = N;
        ex_FragPos=FragPos[i];
        EmitVertex( );
    }
    
    EndPrimitive( );
}