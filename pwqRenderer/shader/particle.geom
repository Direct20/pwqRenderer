#version 330 core
layout(points) in;
layout(triangle_strip,max_vertices=3) out;
uniform float parSize=0.5;
//in vec2 texCoord[];
//out vec2 texcrd;
void main()
{
	vec2 directions[4]={vec2(-1.-1),vec2(-1,1),vec2(1,1),vec2(1,-1)};

	for(int i=0;i<3;i++)
	{
		gl_Position=vec4(gl_in[0].gl_Position.xy+directions[i]*parSize,gl_in[0].gl_Position.z,1.f);
		EmitVertex();
	}
	//texcrd=texCoord[0];
	EndPrimitive();
}