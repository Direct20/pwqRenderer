#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec4 aColor;
layout(location=2) in vec2 aTexCoord;
layout(location=3) in vec3 aNormal;
struct Transform
{
	mat4 model;
	mat4 view;
	mat4 projection;
};
uniform Transform trans;

out VS_OUT
{
	vec3 vertNormal;
	mat4 vertView;
	mat4 vertProjection;
}vsout;
void main()
{
	gl_Position=trans.projection*trans.view*trans.model*vec4(aPos,1.);
	vsout.vertNormal=(trans.model*vec4(aNormal,0.)).xyz;
	vsout.vertView=trans.view;
	vsout.vertProjection=trans.projection;
}