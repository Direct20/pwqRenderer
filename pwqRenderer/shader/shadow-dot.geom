#version 330 core
layout(triangles) in;
layout(triangle_strip,max_vertices=18) out;

#define CNT_FACES   6

struct Transf
{
	mat4 model;
	mat4 projection;
};
uniform Transf trans;

uniform mat4 light[6];

out vec4 fragPos;
void main()
{
	for(int face = 0; face < CNT_FACES; ++face)
	{
		gl_Layer = face;//²ãË÷Òý
		for(int i = 0; i < 3; ++i)
		{
			fragPos = gl_in[i].gl_Position;
			gl_Position = trans.projection*light[face] * fragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}