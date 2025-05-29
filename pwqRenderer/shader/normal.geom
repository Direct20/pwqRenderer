#version 330 core
layout(triangles) in;
layout(line_strip,max_vertices=6) out;

in VS_OUT
{
    vec3 vertNormal;
   	mat4 vertView;
	mat4 vertProjection;
}gsin[];
void makeNormalLine(int index)
{
    vec4 norm=gsin[index].vertProjection*gsin[index].vertView*vec4(normalize(gsin[index].vertNormal),0.);
    vec4 begin=gl_in[index].gl_Position;
    vec4 end=begin+norm*0.2;
    gl_Position = begin;
    EmitVertex();
    gl_Position = end;
    EmitVertex();
    EndPrimitive();
}
void main()
{
    makeNormalLine(0);
    makeNormalLine(1);
    makeNormalLine(2);
}